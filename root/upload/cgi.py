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



body = []
for line in sys.stdin:
    body.append(line)

boundary = body[0][:-2]
file_name = body[1][body[1].rfind('=') + 2:-3]
body_found = False
for line in body:
    if body_found:
        if boundary in line:
            new_file.close()
        elif boundary not in line:
            new_file.write(line.encode("utf-8", "surrogateescape"))
    elif line == "\r\n":
        new_file = open(file_name, 'wb')
        body_found = True

# Split the body string into parts using the boundary string


# Iterate over the parts and extract the file data
