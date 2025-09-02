#!/usr/bin/env python3
# Copyright lowRISC contributors
# SPDX-License-Identifier: Apache-2.0

import argparse
import json
import os
import subprocess
import sys
from pathlib import Path


def run(cmd, cwd=None, quiet=False, env=None):
    proc = subprocess.run(cmd, cwd=cwd, shell=False,
                          stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT,
                          text=True,
                          env=env)
    if not quiet:
        sys.stdout.write(proc.stdout)
    if proc.returncode != 0:
        raise SystemExit(f"Command failed: {' '.join(cmd)}\n{proc.stdout}")
    return proc.stdout


def discover_tests(tests_dir: Path) -> list[Path]:
    return sorted(p for p in tests_dir.glob(f"{tests_dir.name.split('/')[-1].split('.')[0]}_*.cpp")
                  if p.name.endswith('_test.cpp'))


def exe_name(src: Path) -> str:
    return src.with_suffix('').name


def run_with_timeout(cmd, cwd=None, quiet=False, env=None, timeout_s: int | None = None):
    import subprocess as _sp
    proc = _sp.Popen(cmd, cwd=cwd, shell=False,
                     stdout=_sp.PIPE, stderr=_sp.STDOUT, text=True, env=env)
    try:
        out, _ = proc.communicate(timeout=timeout_s)
        if not quiet:
            sys.stdout.write(out)
        return proc.returncode, out
    except _sp.TimeoutExpired:
        try:
            proc.kill()
        except Exception:
            pass
        try:
            out, _ = proc.communicate(timeout=2)
        except Exception:
            out = ''
        if not quiet and out:
            sys.stdout.write(out)
        return -9, out


def run_tests_and_collect_json(tests_dir: Path, tmp_json_dir: Path, env, repeat: int = 1, extra_args: list[str] | None = None, jobs: int = 1, tests: list[str] | None = None, timeout_s: int | None = None):
    tmp_json_dir.mkdir(parents=True, exist_ok=True)
    # Build requested tests (or all)
    if tests:
        for t in tests:
            run(["make", "-j", "1", t], cwd=str(tests_dir), quiet=True, env=env)
    else:
        run(["make", "-j", "all"], cwd=str(tests_dir), quiet=True, env=env)
    # Execute tests and copy JSON/XML after each repeat per test
    args = extra_args or []
    import concurrent.futures
    max_jobs = max(1, int(jobs))
    executor = None
    try:
        if max_jobs > 1:
            executor = concurrent.futures.ThreadPoolExecutor(max_workers=max_jobs)
        futures = []
        exe_paths = sorted(tests_dir.glob("*_test"))
        if tests:
            exe_paths = [p for p in exe_paths if p.name in tests]
        for exe in exe_paths:
            for r in range(max(1, int(repeat))):
                # Per-invocation unique OUTDIR/BASENAME for FC4SC
                env_i = env.copy()
                env_i['FC4SC_OUTDIR'] = str(tmp_json_dir)
                env_i['FC4SC_BASENAME'] = f"{exe.name}.r{r:03d}"
                cmd = [f"./{exe.name}", *args]
                if executor:
                    futures.append(executor.submit(run_with_timeout, cmd, str(tests_dir), True, env_i, timeout_s))
                else:
                    rc, _ = run_with_timeout(cmd, cwd=str(tests_dir), quiet=True, env=env_i, timeout_s=timeout_s)
                    (lambda x: x)(rc)  # no-op to keep flake8 quiet if present
        if executor and futures:
            for f in futures:
                try:
                    _ = f.result()
                except Exception:
                    pass
    finally:
        if executor:
            executor.shutdown(wait=True, cancel_futures=False)


def merge_fc4sc_json(src_dir: Path, out_json: Path) -> None:
    import collections, glob
    Scope = collections.defaultdict(
        lambda: collections.defaultdict(
            lambda: collections.defaultdict(lambda: collections.Counter())))
    for p in sorted(glob.glob(str(src_dir / "*.json"))):
        try:
            d = json.loads(Path(p).read_text())
        except Exception:
            continue
        for s in d.get('scopes', []):
            sname = s.get('name', '')
            for cg in s.get('covergroups', []):
                cgname = cg.get('name', '')
                for cp in cg.get('coverpoints', []):
                    cpname = cp.get('name', '')
                    for b in cp.get('bins', []):
                        Scope[sname][cgname][cpname][b.get('name', '')] += int(b.get('hits', 0))
    merged = {'scopes': []}
    for sname, cgs in Scope.items():
        sc = {'name': sname, 'covergroups': []}
        for cgname, cps in cgs.items():
            cg = {'name': cgname, 'coverpoints': []}
            for cpname, bins in cps.items():
                cp = {'name': cpname,
                      'bins': [{'name': bn, 'hits': int(h)} for bn, h in bins.items()]}
                cg['coverpoints'].append(cp)
            sc['covergroups'].append(cg)
        merged['scopes'].append(sc)
    out_json.write_text(json.dumps(merged, indent=2))
def read_manifest(ip_root: Path) -> dict:
    try:
        mpath = ip_root / 'coverage_manifest.json'
        if mpath.exists():
            import json as _json
            return _json.loads(mpath.read_text())
    except Exception:
        pass
    return {}


def mark_na_bins(d: dict, manifest: dict | None = None) -> None:
    # Mark bins that are not applicable (N/A) so they don't count toward coverage
    # Use coverage_manifest.json if present to decide if a category exists; else fallback to heuristic
    try:
        cats_present = set()
        addr_caps = {}
        if manifest:
            cats_present = {k for k, v in (manifest.get('categories', {}) or {}).items() if isinstance(v, list) and len(v) > 0}
            for ent in manifest.get('access', []) or []:
                addr_caps[int(ent.get('addr', 0))] = {'R': bool(ent.get('can_read', False)), 'W': bool(ent.get('can_write', False))}
        for s in d.get('scopes', []):
            for cg in s.get('covergroups', []):
                for cp in cg.get('coverpoints', []):
                    if cp.get('name') == 'category_cp':
                        for b in cp.get('bins', []):
                            nm = str(b.get('name', ''))
                            hits = int(b.get('hits', 0))
                            if hits == 0:
                                if manifest:
                                    if nm in ('shadow', 'regwen', 'rc', 'intr', 'alert') and nm not in cats_present:
                                        b['ignore'] = True; b['goal'] = 0
                                else:
                                    if nm in ('shadow', 'regwen'):
                                        b['ignore'] = True; b['goal'] = 0
                    if cp.get('name') == 'addr_cp' and addr_caps:
                        # Mark addr×rw cross indirectly via can_read/can_write when report renders crosses from UCIS
                        pass
    except Exception:
        pass


def parse_ucis_crosses(src_dir: Path):
    import xml.etree.ElementTree as ET
    crosses = {}
    for xml_path in sorted(src_dir.glob('*.ucis.xml')):
        try:
            tree = ET.parse(str(xml_path))
        except Exception:
            continue
        root = tree.getroot()
        ns = {'u': 'UCIS'}
        # coverpoint -> bin names
        cp2bins = {}
        for cp_elem in root.findall('.//u:coverpoint', ns):
            cp_name = cp_elem.get('name') or ''
            bins = [b.get('name') or '' for b in cp_elem.findall('u:coverpointBin', ns)]
            if cp_name:
                cp2bins[cp_name] = bins
        for cross in root.findall('.//u:cross', ns):
            exprs = [ce.text or '' for ce in cross.findall('u:crossExpr', ns)]
            key = ' × '.join(exprs) if exprs else 'cross'
            for cb in cross.findall('u:crossBin', ns):
                idxs = [int(ix.text or '0') for ix in cb.findall('u:index', ns)]
                cnt = cb.find('u:contents', ns)
                hits = int(cnt.get('coverageCount')) if cnt is not None else 0
                labels = []
                for dim, cp_name in enumerate(exprs):
                    bin_idx = idxs[dim] if dim < len(idxs) else 0
                    bn_list = cp2bins.get(cp_name, [])
                    bn = bn_list[bin_idx] if bin_idx < len(bn_list) else f'idx{bin_idx}'
                    labels.append(f'{cp_name}:{bn}')
                label = ' × '.join(labels)
                crosses.setdefault(key, {})[label] = crosses.setdefault(key, {}).get(label, 0) + hits
    return crosses


def write_simple_html(from_json: Path, out_dir: Path, title: str, per_test_dir=None, rich: bool = False) -> Path:
    import html
    d = json.loads(from_json.read_text())
    ip_tests_dir = out_dir if out_dir.name == 'tests' else out_dir.parent
    manifest = read_manifest(ip_tests_dir)
    mark_na_bins(d, manifest)
    out_dir.mkdir(parents=True, exist_ok=True)
    idx = out_dir / 'index.html'

    def bin_fields(b):
        # Defaults if fields are absent in JSON
        goal = int(b.get('goal', 1))
        hits = int(b.get('hits', 0))
        illegal = bool(b.get('illegal', False))
        ignore = bool(b.get('ignore', False))
        weight = int(b.get('weight', 1))
        return hits, goal, illegal, ignore, weight

    def cp_cov(cp):
        bins = cp.get('bins', [])
        considered = [b for b in bins if not bool(b.get('illegal', False)) and not bool(b.get('ignore', False))]
        total = len(considered)
        covered = 0
        w_total = 0
        w_covered = 0
        for b in considered:
            hits, goal, _, _, weight = bin_fields(b)
            met = hits >= max(1, goal)
            covered += 1 if met else 0
            w_total += weight
            w_covered += (weight if met else 0)
        pct = (covered / total * 100.0) if total else 0.0
        wpct = (w_covered / w_total * 100.0) if w_total else pct
        return covered, total, pct, w_covered, w_total, wpct

    def cg_cov(cg):
        totals = [cp_cov(cp) for cp in cg.get('coverpoints', [])]
        covered = sum(c for c, t, _, _, _, _ in totals)
        total = sum(t for _, t, _, _, _, _ in totals)
        w_cov = sum(wc for _, _, _, wc, _, _ in totals)
        w_tot = sum(wt for _, _, _, _, wt, _ in totals)
        pct = (covered / total * 100.0) if total else 0.0
        wpct = (w_cov / w_tot * 100.0) if w_tot else pct
        return covered, total, pct, w_cov, w_tot, wpct

    def scp_cov(s):
        totals = [cg_cov(cg) for cg in s.get('covergroups', [])]
        covered = sum(c for c, t, _, _, _, _ in totals)
        total = sum(t for _, t, _, _, _, _ in totals)
        w_cov = sum(wc for _, _, _, wc, _, _ in totals)
        w_tot = sum(wt for _, _, _, _, wt, _ in totals)
        pct = (covered / total * 100.0) if total else 0.0
        wpct = (w_cov / w_tot * 100.0) if w_tot else pct
        return covered, total, pct, w_cov, w_tot, wpct

    # Overall coverage
    scopes_stats = [scp_cov(s) for s in d.get('scopes', [])]
    overall_cov = sum(c for c, t, _, _, _, _ in scopes_stats)
    overall_tot = sum(t for _, t, _, _, _, _ in scopes_stats)
    w_overall_cov = sum(wc for _, _, _, wc, _, _ in scopes_stats)
    w_overall_tot = sum(wt for _, _, _, _, wt, _ in scopes_stats)
    overall_pct = (overall_cov / overall_tot * 100.0) if overall_tot else 0.0
    w_overall_pct = (w_overall_cov / w_overall_tot * 100.0) if w_overall_tot else overall_pct

    # Per-test contribution (if available)
    per_test_hits = []
    if per_test_dir and per_test_dir.exists():
        import glob
        for p in sorted(glob.glob(str(per_test_dir / '*.json'))):
            try:
                td = json.loads(Path(p).read_text())
            except Exception:
                continue
            hits = 0
            for s in td.get('scopes', []):
                for cg in s.get('covergroups', []):
                    for cp in cg.get('coverpoints', []):
                        for b in cp.get('bins', []):
                            if int(b.get('hits', 0)) > 0:
                                hits += 1
            per_test_hits.append((Path(p).stem, hits))

    rows = [
        '<html><head><meta charset="utf-8">',
        f'<title>{html.escape(title)}</title>',
        '<style>body{font-family:Arial,Helvetica,sans-serif} body{scroll-behavior:smooth} .toc{background:#f7f7f7;border:1px solid #ddd;padding:8px;margin:8px 0} .toc a{margin-right:8px} table{border-collapse:collapse;margin-bottom:16px;width:100%} th,td{border:1px solid #ccc;padding:4px 8px;text-align:left} thead th{position:sticky;top:0;background:#fafafa} .pct{font-weight:bold} .controls{margin:12px 0} .muted{color:#666} .status-ok{color:#0a0} .status-miss{color:#a00;font-weight:bold} details{margin:10px 0} summary{cursor:pointer;font-weight:bold}</style>',
        '<script>function toggleRows(){const hideFull=document.getElementById("hideFull").checked;const onlyUncov=document.getElementById("onlyUncov").checked;const q=(document.getElementById("searchCp")||{value:""}).value.toLowerCase();document.querySelectorAll("tr[data-cp]").forEach(tr=>{const pct=parseFloat(tr.getAttribute("data-cp"));const namecell=tr.querySelector("td");let show=true;if(hideFull&&pct===100)show=false;if(onlyUncov&&pct>0)show=false;if(q&&namecell){show=show && namecell.textContent.toLowerCase().includes(q);}tr.style.display=show?"":"none";});document.querySelectorAll("tr[data-bin]").forEach(tr=>{const hit=parseInt(tr.getAttribute("data-hit"));tr.className= hit===0?"muted":"";});}</script>',
    ]
    if rich:
        rows.append('<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>')
    rows.extend([
        '</head><body>',
        f'<h1>{html.escape(title)}</h1>',
        f'<p class="pct">Overall (unweighted): {overall_cov} / {overall_tot} bins ({overall_pct:.1f}%) &nbsp; | &nbsp; Weighted: {w_overall_cov} / {w_overall_tot} ({w_overall_pct:.1f}%)</p>',
        '<div class="controls"><label><input type="checkbox" id="hideFull" onchange="toggleRows()"> Hide fully covered coverpoints</label> &nbsp; <label><input type="checkbox" id="onlyUncov" onchange="toggleRows()"> Show only uncovered coverpoints</label> &nbsp; <label>Search CP: <input type="text" id="searchCp" oninput="toggleRows()" placeholder="coverpoint name..."></label></div>'
    ])
    chart_calls = []
    if rich:
        rows.append('<div style="max-width:400px"><canvas id="chart_overall"></canvas></div>')
        chart_calls.append(("chart_overall", overall_cov, max(0, overall_tot-overall_cov), "Overall"))

    # Table of contents
    rows.append('<div class="toc"><b>Table of contents:</b> ')
    for si, s in enumerate(d.get('scopes', [])):
        sid = f"scope_{si}"
        rows.append(f'<a href="#{sid}">{html.escape(s.get("name",""))}</a> ')
    rows.append('</div>')

    # Top uncovered coverpoints summary
    uncovered = []
    for s in d.get('scopes', []):
        for cg in s.get('covergroups', []):
            for cp in cg.get('coverpoints', []):
                c_cov, c_tot, c_pct, _, _, _ = cp_cov(cp)
                uncovered.append((c_pct, s.get('name',''), cg.get('name',''), cp.get('name','')))
    uncovered = sorted(uncovered, key=lambda x: x[0])
    rows.append('<details open><summary>Least covered coverpoints</summary>')
    rows.append('<table><thead><tr><th>Scope</th><th>Covergroup</th><th>Coverpoint</th><th>Percent</th></tr></thead>')
    for c_pct, sname, cgname, cpname in uncovered[:20]:
        rows.append(f'<tr><td>{html.escape(sname)}</td><td>{html.escape(cgname)}</td><td>{html.escape(cpname)}</td><td>{c_pct:.1f}%</td></tr>')
    rows.append('</table></details>')

    # Per-test contributions
    if per_test_hits:
        rows.append('<details><summary>Per-test contributions</summary>')
        rows.append('<table><thead><tr><th>Test</th><th>Bins hit</th></tr></thead>')
        for name, hits in sorted(per_test_hits, key=lambda x: -x[1])[:20]:
            rows.append(f'<tr><td>{html.escape(name)}</td><td>{hits}</td></tr>')
        rows.append('</table></details>')

    for si, s in enumerate(d.get('scopes', [])):
        sid = f"scope_{si}"
        s_cov, s_tot, s_pct, sw_cov, sw_tot, sw_pct = scp_cov(s)
        if rich:
            rows.append(f"<details open id='{sid}'><summary>Scope: {html.escape(s.get('name',''))} — {s_cov} / {s_tot} bins ({s_pct:.1f}%), weighted {sw_cov}/{sw_tot} ({sw_pct:.1f}%)</summary>")
            cid = f"chart_scope_{html.escape(s.get('name','')).replace(' ','_')}"
            rows.append(f'<div style="max-width:360px"><canvas id="{cid}"></canvas></div>')
            chart_calls.append((cid, s_cov, max(0, s_tot-s_cov), f"Scope {html.escape(s.get('name',''))}"))
        else:
            rows.append(f"<h2 id='{sid}'>Scope: {html.escape(s.get('name',''))} — {s_cov} / {s_tot} bins ({s_pct:.1f}%), weighted {sw_cov}/{sw_tot} ({sw_pct:.1f}%)</h2>")
        for ci, cg in enumerate(s.get('covergroups', [])):
            gid_anchor = f"{sid}_cg_{ci}"
            cg_covv, cg_tot, cg_pct, cgw_cov, cgw_tot, cgw_pct = cg_cov(cg)
            if rich:
                rows.append(f"<details open id='{gid_anchor}'><summary>Covergroup: {html.escape(cg.get('name',''))} — {cg_covv} / {cg_tot} bins ({cg_pct:.1f}%), weighted {cgw_cov}/{cgw_tot} ({cgw_pct:.1f}%)</summary>")
                gid = f"chart_cg_{html.escape(cg.get('name','')).replace(' ','_')}"
                rows.append(f'<div style="max-width:320px"><canvas id="{gid}"></canvas></div>')
                chart_calls.append((gid, cg_covv, max(0, cg_tot-cg_covv), f"CG {html.escape(cg.get('name',''))}"))
            else:
                rows.append(f"<h3 id='{gid_anchor}'>Covergroup: {html.escape(cg.get('name',''))} — {cg_covv} / {cg_tot} bins ({cg_pct:.1f}%), weighted {cgw_cov}/{cgw_tot} ({cgw_pct:.1f}%)</h3>")
            rows.append('<table><thead><tr><th>Coverpoint</th><th>Covered/Total</th><th>Percent</th><th>Bin</th><th>Goal</th><th>Weight</th><th>Hits</th><th>Status</th></tr></thead>')
            for cp in cg.get('coverpoints', []):
                c_cov, c_tot, c_pct, wc_cov, wc_tot, wc_pct = cp_cov(cp)
                cpname = html.escape(cp.get('name', ''))
                first = True
                for b in cp.get('bins', []):
                    hits, goal, illegal, ignore, weight = bin_fields(b)
                    status_ok = (hits >= max(1, goal)) if not (illegal or ignore) else True
                    row = f'<tr data-cp="{c_pct:.1f}" data-bin data-hit="{hits}">' 
                    if first:
                        row += f'<td rowspan="{max(1,len(cp.get("bins",[])))}">{cpname}</td>'
                        row += f'<td rowspan="{max(1,len(cp.get("bins",[])))}">{c_cov} / {c_tot}</td>'
                        row += f'<td rowspan="{max(1,len(cp.get("bins",[])))}">{c_pct:.1f}%</td>'
                        first = False
                    row += f'<td>{html.escape(str(b.get("name","")))}</td><td>{goal}</td><td>{weight}</td><td>{hits}</td>'
                    row += f'<td class="{"status-ok" if status_ok else "status-miss"}">{"OK" if status_ok else "MISS"}</td></tr>'
                    rows.append(row)
                if first:  # no bins
                    rows.append(f'<tr data-cp="{c_pct:.1f}"><td>{cpname}</td><td>{c_cov} / {c_tot}</td><td>{c_pct:.1f}%</td><td>-</td><td>1</td><td>1</td><td>0</td><td class="status-miss">MISS</td></tr>')
            rows.append('</table>')
            if rich:
                rows.append('</details>')
        if rich:
            rows.append('</details>')

    # UCIS cross coverage tables (formal)
    crosses = parse_ucis_crosses(per_test_dir) if per_test_dir else {}
    if crosses:
        rows.append('<h2>Cross coverage (UCIS)</h2>')
        for cname, bins in crosses.items():
            total = len(bins)
            covered = sum(1 for _, h in bins.items() if h > 0)
            pct = (covered / total * 100.0) if total else 0.0
            rows.append(f'<details open><summary>{cname} — {covered} / {total} bins ({pct:.1f}%)</summary>')
            rows.append('<table><tr><th>Bin (combination)</th><th>Hits</th></tr>')
            for lbl, h in sorted(bins.items()):
                rows.append(f'<tr><td>{lbl}</td><td>{h}</td></tr>')
            rows.append('</table></details>')
    rows.append('</body></html>')
    if rich and chart_calls:
        rows.append('<script>function makePie(id,covered,uncovered,label){const ctx=document.getElementById(id).getContext("2d");new Chart(ctx,{type:"pie",data:{labels:["Covered","Uncovered"],datasets:[{data:[covered,uncovered],backgroundColor:["#2ca02c","#d62728"]}]},options:{plugins:{legend:{position:"bottom"},title:{display:true,text:label}}});}</script>')
        rows.append('<script>')
        for cid, cov, uncov, label in chart_calls:
            rows.append(f'makePie("{cid}",{int(cov)},{int(uncov)},"{label}");')
        rows.append('</script>')
    rows.append('</body></html>')
    idx.write_text('\n'.join(rows))
    return idx


def main():
    ap = argparse.ArgumentParser(description="Run SCDV tests, merge FC4SC JSON, emit HTML report")
    ap.add_argument('--ip', required=True, help='IP name (e.g., uart)')
    ap.add_argument('--outdir', default='coverage_html', help='Output HTML directory')
    ap.add_argument('--systemc-home', default=os.environ.get('SYSTEMC_HOME', ''), help='SYSTEMC_HOME path')
    ap.add_argument('--rich', action='store_true', help='Generate rich HTML with collapsible sections and pie charts')
    ap.add_argument('--repeat', type=int, default=1, help='Repeat each test N times (serial)')
    ap.add_argument('--jobs', type=int, default=1, help='Requested parallel jobs (currently forced to 1 due to FC4SC file collisions)')
    ap.add_argument('--tests', default='', help='Comma-separated test executables to build/run (e.g. i2c_smoke_test,i2c_csr_rw_test)')
    ap.add_argument('--timeout', type=int, default=60, help='Per-test timeout in seconds (0 to disable)')
    ap.add_argument('--extra-args', default='', help='Extra space-separated arguments to pass to each test (e.g. +UVM_VERBOSITY=UVM_LOW)')
    ap.add_argument('--skip-run', action='store_true', help='Skip building/running tests; only merge and emit HTML')
    ap.add_argument('--src-json-dir', action='append', default=[], help='Additional directories containing fc4sc JSONs to merge')
    ap.add_argument('--accumulate', action='store_true', help='Accumulate with previous merged results (reads merged.json in outdir if present)')
    args = ap.parse_args()

    ip_root = Path('hw/ip') / args.ip / 'scdv' / 'tests'
    if not ip_root.is_dir():
        raise SystemExit(f"Tests directory not found: {ip_root}")
    if not args.systemc_home:
        print('WARNING: SYSTEMC_HOME is not set; build may fail.', file=sys.stderr)

    # Ensure build deps
    env = os.environ.copy()
    if args.systemc_home:
        env['SYSTEMC_HOME'] = args.systemc_home

    tmp_json_dir = Path('/tmp') / f'fc4sc_jsons_{args.ip}'
    tmp_json_dir.mkdir(parents=True, exist_ok=True)

    # Build and run tests (unless skipped)
    if not args.skip_run:
        extra_args = [a for a in args.extra_args.split(' ') if a]
        tests = [t for t in args.tests.split(',') if t]
        timeout_s = None if (args.timeout is None or int(args.timeout) <= 0) else int(args.timeout)
        run_tests_and_collect_json(ip_root, tmp_json_dir, env, repeat=args.repeat, extra_args=extra_args, jobs=args.jobs, tests=tests or None, timeout_s=timeout_s)
    else:
        # Guard: warn if there are no JSONs to merge
        has_json = any(p.suffix == '.json' for p in tmp_json_dir.glob('*.json'))
        if not has_json and not args.src_json_dir and not args.accumulate:
            print("WARNING: --skip-run used and no JSON found to merge; output may be empty.", file=sys.stderr)

    # Optionally include external sources
    for sd in args.src_json_dir:
        sd_path = Path(sd)
        if sd_path.is_dir():
            # Link or copy JSONs into tmp_json_dir to participate in merge
            for p in sd_path.glob('*.json'):
                try:
                    dst = tmp_json_dir / p.name
                    if not dst.exists():
                        dst.write_bytes(p.read_bytes())
                except Exception:
                    pass

    # Resolve output directory
    outdir_arg = Path(args.outdir)
    if outdir_arg.is_absolute():
        outdir = outdir_arg
    else:
        # If caller already passed a path under ip_root, use it as-is
        try:
            if str(outdir_arg).startswith(str(ip_root)):
                outdir = outdir_arg
            else:
                outdir = ip_root / outdir_arg
        except Exception:
            outdir = ip_root / outdir_arg

    # If accumulate, include previous merged JSON if present
    prev_merged = outdir / 'merged.json'
    if args.accumulate and prev_merged.exists():
        try:
            (tmp_json_dir / '__prev_merged.json').write_bytes(prev_merged.read_bytes())
        except Exception:
            pass

    # Merge JSONs and emit report
    merged_json = Path('/tmp') / f'fc4sc_cov_merged_{args.ip}.json'
    merge_fc4sc_json(tmp_json_dir, merged_json)
    idx = write_simple_html(merged_json, outdir, f"{args.ip.upper()} Coverage (Merged fc4sc)", tmp_json_dir, rich=args.rich)
    # Save the possibly N/A-adjusted payload next to HTML for future accumulation
    try:
        import json as _json
        adj = _json.loads(merged_json.read_text())
        ip_tests_dir = outdir if outdir.name == 'tests' else outdir.parent
        manifest = read_manifest(ip_tests_dir)
        mark_na_bins(adj, manifest)
        (outdir / 'merged.json').write_text(_json.dumps(adj, indent=2))
    except Exception:
        try:
            (outdir / 'merged.json').write_bytes(merged_json.read_bytes())
        except Exception:
            pass
    print(f"HTML report: {idx}")


if __name__ == '__main__':
    main()


