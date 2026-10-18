"""Locate the local-only extraction without copying binaries into the saved kit."""
from pathlib import Path
import json
def target_path(root):
 root=Path(root)
 local=root/'orig/1ST_READ.BIN'
 if local.is_file():return local
 cfg=json.loads((root/'config/target.json').read_text(encoding='utf-8-sig'))
 path=Path(cfg['working_copy'])
 if not path.is_file():raise FileNotFoundError('Local scratch extraction missing; set config/target.json working_copy to a verified local extraction. Expected SHA256: '+cfg['sha256'])
 return path
