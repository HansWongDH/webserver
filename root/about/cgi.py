#!/usr/bin/env python3

import os

# Parse the query string for variables
query_string = os.environ.get("QUERY_STRING", "")
variables = {}
if query_string:
    for pair in query_string.split("&"):
        name, value = pair.split("=")
        variables[name] = value

# Get the value of the "name" variable
name = variables.get("name", "")

# Print the HTML document
print("<html>")
print("<head>")
print("<title>CGI Script in Python</title>")
print("</head>")
print("<body>")
print("<h1>Hello, " + name + "!</h1>")
print("</body>")
print("</html>")