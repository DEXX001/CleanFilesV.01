# CleanFiles — Directory Analysis & Optimization Tool
------
*Work in progress*
------

CleanFiles is a C-based system utility designed to scan folders, detect unnecessary files, and prepare automated cleanup actions.
It forms the foundation of a larger optimization suite called DEXPACK (DXP).

### Core Features
• Fast directory scanning

Efficiently traverses a folder (and its subfolders).

• Full content listing

Retrieves all files and subdirectories with their metadata.

• Detailed file analysis

Uses stat() and struct dirent to extract type, size, timestamps, and more.

• Detection of useless or heavy files

Identifies temporary files, duplicates, large files, and unnecessary items.

• Foundation for future automation

Lays the groundwork for:

automatic deletion

archiving

advanced logging

future GUI versions

### Version Roadmap
V0.x — CLI Version (current)

✔ Directory scanning
✔ File analysis (type, size, timestamps)
✔ Unnecessary file detection
➡ Strong technical base for upcoming versions

