CleanFiles — Directory Analysis & Optimization Tool

CleanFiles is a system utility fully developed in C.
Its purpose is simple and efficient: scan a directory, identify unnecessary files, and prepare future automated actions.
It serves as the foundation of a larger suite of optimization tools that will form DEXPACK (DXP).

🔍 Core Features

Fast directory scanning
Efficiently traverses a folder (and subfolders).

Full content listing
Retrieves all files and subdirectories with their basic metadata.

Detailed file analysis
Uses stat() and struct dirent to extract type, size, timestamps, and other properties.

Detection of useless or heavy files
Identifies temporary files, duplicates, large files, and items that can be cleaned.

Foundation for upcoming automation
Lays the groundwork for:
• automatic deletion
• archiving
• advanced logging
• future GUI versions

🛠️ Version Roadmap

V0.x — CLI Version (current)
✔ Directory scanning
✔ File analysis (type, size, etc.)
✔ Detection of unnecessary files
➤ Solid technical base for upcoming features

V1 — Full Optimization Engine
• Advanced detection rules
• Cleaner terminal output
• Configurable filters
• Basic logs
• Ready for integration into DEXPACK
