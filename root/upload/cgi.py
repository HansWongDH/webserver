import os

# Get the boundary string from the Content-Type header
content_type = os.environ["CONTENT_TYPE"]
boundary = content_type.split("=")[1].encode()

# Get the body string from the environment variable
body_string = os.environ["BODY_STRING"].encode()

# Split the body string into parts using the boundary string
parts = body_string.split(boundary)

# Iterate over the parts and extract the file data
for part in parts:
    if b"filename" in part:
        # Get the filename from the Content-Disposition header
        filename_pos = part.find(b"filename")
        filename = part[filename_pos + 10 : part.find(b"\r\n", filename_pos)].decode().strip('"')

        # Get the file data from the part
        file_pos = part.find(b"\r\n\r\n") + 4
        file_data = part[file_pos : part.find(b"\r\n", file_pos)]

        # Save the file data to disk
        with open(filename, "wb") as f:
            f.write(file_data)