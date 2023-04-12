import os
import sys

# Get the boundary string from the Content-Type header
# content_type = os.environ["Content-Type"]

# def	save_file(full_path, body, boundary):
# 	header_done = False
# 	for line in body:
# 		if "filename" in line:
# 			name = line[line.rfind('=') + 2:-3]
# 		if header_done:
# 			if ("--" + boundary + "--") in line:
# 				new_file.close()
# 			elif boundary not in line:
# 				new_file.write(line.encode("utf-8", "surrogateescape"))
# 		elif line == "\r\n":
# 			new_file = open(full_path + "/" + name, 'wb')
# 			header_done = True
# # Get the body string from the environment variable


# try:
# body = []
# for line in sys.stdin:
#     print(line)
#     body.append(line)

# boundary = body[0][:-2]
# file_name = body[1][body[1].rfind('=') + 2:-3]
# body_found = False
# for line in body:
#     if body_found:
#         if boundary in line:
#             new_file.close()
#         elif boundary not in line:
#             new_file.write(line.encode("utf-8", "surrogateescape"))
#     elif line == "\r\n":
#         new_file = open(file_name, 'wb')
#         body_found = True
# except:
#     pass
# Split the body string into parts using the boundary string


# Iterate over the parts and extract the file data
# print(os.environ["SIZE"])
# body = []
# for line in sys.stdin.buffer:
#     print(line)
#     body.append(line)

# boundary = body[0][:-2]
# file_name = body[1][body[1].rfind('=') + 2:-3]
# print(file_name)
# body_found = False
# new_file = None
# for line in body:
#     if body_found:
#         if boundary in line:
#             new_file.close()
#         elif boundary not in line:
#             new_file.write(line.encode('utf-8', "surrogateespace"))
#     elif line == "\r\n":
#         new_file = open(file_name, 'wb')
#         body_found = True

# import sys
# import os

# CHUNK_SIZE = int(os.environ["SIZE"])
# print(CHUNK_SIZE)
# def read_chunks():
#     while True:
#         chunk = sys.stdin.buffer.read(CHUNK_SIZE)
#         if not chunk:
#             break
#         yield chunk

# body = b"".join(read_chunks())

# boundary_index = body.find(b"\r\n")
# boundary = body[:boundary_index].strip()
# file_name_index_start = body.find(b"filename=") + len(b"filename=") + 1
# file_name_index_end = body.find(b"\r\n", file_name_index_start) - 1
# file_name = body[file_name_index_start:file_name_index_end].decode('utf-8')

# print(file_name)

# data_start_index = body.find(b"\r\n\r\n", file_name_index_end) + 4
# data_end_index = body.find(boundary + b"--", data_start_index) - 2

# with open(file_name, 'wb') as new_file:
#     new_file.write(body[data_start_index:data_end_index])

# import sys
# import os

# CHUNK_SIZE = int(os.environ["SIZE"])

# def read_chunk():
#     return sys.stdin.buffer.read(CHUNK_SIZE)

# def process_body_chunk(chunk, new_file):
#     global data_start_index, data_end_index
#     data_start_index = 0
#     while True:
#         data_end_index = chunk.find(boundary + b"--", data_start_index)
#         if data_end_index != -1:
#             break
#         new_file.write(chunk[data_start_index:])
#         chunk = read_chunk()
#         if not chunk:
#             break
#         data_start_index = 0
#     new_file.write(chunk[data_start_index:data_end_index])
#     return chunk[data_end_index:]

# boundary = sys.stdin.buffer.readline().strip()

# file_name = "output.bin"
# while True:
#     line = sys.stdin.buffer.readline().strip()
#     if not line:
#         break
#     if b"filename=" in line:
#         file_name_start = line.find(b"filename=") + len(b"filename=") + 1
#         file_name_end = line.find(b'"', file_name_start)
#         file_name = line[file_name_start:file_name_end].decode('utf-8')
#         break

# print(file_name)

# with open(file_name, 'wb') as new_file:
#     while True:
#         chunk = read_chunk()
#         if not chunk:
#             break
#         chunk = process_body_chunk(chunk, new_file)
#         if boundary + b"--" in chunk:
#             break

import sys

def find_boundary_end(boundary, body):
    for i in range(len(body)):
        if body[i:i+len(boundary)+2] == boundary + b'--':
            return i
    return -1

body = b''
length = 0
while True:
    data = sys.stdin.buffer.read(int(os.environ["SIZE"]))
    if not data:
        break
    body += data
    length += len(data)

print("Received length:", len(body), file=sys.stderr)

body_lines = body.split(b'\r\n')
boundary = body_lines[0]
file_name = body_lines[1][body_lines[1].rfind(b'=') + 2:-1]

boundary_end_pos = find_boundary_end(boundary, body)
print("Found boundary end", boundary_end_pos, file=sys.stderr)
if boundary_end_pos == -1:
    raise ValueError("Boundary not found")

body_content = body[len(boundary)+2:boundary_end_pos].split(b'\r\n')[2:]
with open(file_name.decode('utf-8'), 'wb') as new_file:
    for line in body_content:
        new_file.write(line)

