import re
import sys
from pathlib2 import Path

content = Path(sys.argv[1]).read_text()

content = re.sub(r"\\", "/", content)
content = re.sub(r"(src\/)", "", content)

Path(sys.argv[1]).write_text(content)