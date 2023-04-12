import sys

"""
Helper function to find for the last boundary
"""
def find_boundary_end(boundary, body):
    for i in range(len(body)):
        if body[i:i+len(boundary)+2] == boundary + b'--':
            return i
    return -1

# Read the buffer from standard input
body = b''
while True:
    data = sys.stdin.buffer.read(1024)
    if not data:
        break
    body += data

# Find boundary
body_lines = body.split(b'\r\n')
boundary = body_lines[0]
file_name = body_lines[1][body_lines[1].rfind(b'=') + 2:-1]

boundary_end_pos = find_boundary_end(boundary, body)
if boundary_end_pos == -1:
    raise ValueError("Boundary not found")

# Find the start of the file content
start_pos = body.find(b'\r\n\r\n') + 4

# Extract the file content
body_content = body[start_pos:boundary_end_pos]

# Write the content to the file
with open(file_name.decode('utf-8'), 'wb') as new_file:
    new_file.write(body_content)

print("OK")
