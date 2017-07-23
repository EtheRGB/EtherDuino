import re
import sys
from pathlib2 import Path

content = Path(sys.argv[1]).read_text()

content = re.sub(r"\r", "", content)
content = re.sub(r"(\.\.\/src\/)", "", content)
content = re.sub(r"(\.\.\/src \\\n)", "", content)
content = re.sub(r"(\.\.\/doc.* \\\n)", "", content)
content = re.sub(r"(src\/)", "", content)
content = re.sub(r"(cmd.exe)", "sh", content)
content = re.sub(r"(C\:\\Program Files \(x86\)\\Atmel\\Studio\\7.0\\toolchain\\avr8\\avr8-gnu-toolchain\\bin\\)", "", content);
content = re.sub(r"(\.exe)", "", content)
content = re.sub(r"( \: 4\.9\.2)", "", content)

Path(sys.argv[1]).write_text(content)