import angr
import claripy
from pathlib import Path


proj = angr.Project(Path("target"))

sym_arg_size = 28
sym_arg = claripy.BVS("sym_arg", 8 * sym_arg_size)

argv = ["target"]
argv.append(sym_arg)
state = proj.factory.entry_state(args=argv)

simgr = proj.factory.simulation_manager(state)

simgr.explore(find=0x401060, avoid=[0x400c06, 0x400bc7])

found = simgr.found[0]
ans = found.solver.eval(sym_arg, cast_to=bytes)

Path("flag.txt").write_bytes(ans)

print(f"Anser => {ans}")
