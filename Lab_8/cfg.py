import angr
from pathlib import Path
from angrutils import plot_cfg

proj = angr.Project(Path("target"), load_options={"auto_load_libs": False})
main = proj.loader.main_object.get_symbol("main")
start_state = proj.factory.blank_state(addr=main.rebased_addr)
cfg = proj.analyses.CFGEmulated(
    fail_fast=True, starts=[main.rebased_addr], initial_state=start_state
)
plot_cfg(
    cfg, "target", asminst=True, remove_imports=True, remove_path_terminator=True
)
