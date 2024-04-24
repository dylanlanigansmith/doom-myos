import re
import os

def find_includes(directory):
    include_directives = set()
    c_files = [f for f in os.listdir(directory) if f.endswith('.c')]

    for filename in c_files:
        with open(os.path.join(directory, filename), 'r') as f:
            code = f.read()
            # Use regular expression to find #include directives
            includes = re.findall(r'#include\s+<([^>]*)>', code)
            # Add found include paths to the set
            include_directives.update(includes)
    
    return include_directives

code_directory = 'doomgeneric'
include_directives = find_includes(code_directory)




# Print all found include directives
for include_directive in include_directives:
    print(include_directive)