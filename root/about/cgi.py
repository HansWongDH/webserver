#!/usr/bin/env python3

import os

# Parse the query string for variables
query_string = os.environ.get("QUERY_STRING", "")
params = {}
if query_string:
    for pair in query_string.split("&"):
        try:
            key, value = pair.split("=")
            params[key] = value
        except:
            print("No pair")

# Get the value of the "name" query params
name = params.get("name", "")

# Parse the body for variables
body_string = os.environ.get("BODY_STRING", "")
params = {}
if body_string:
    for pair in body_string.split("&"):
        try:
            key, value = pair.split("=")
            params[key] = value
        except:
            print("No pair")

# Get the value of the "name" query params
username = params.get("username", "")
password = params.get("password", "")

# Print the HTML document
print("<html>")
print("<head>")
print("<title>CGI Script in Python</title>")
print("</head>")
print("<body>")
print("<h1>Hello, " + name + "!</h1>")
print("<br />")
print("<h2>Username: " + username + "</h2>")
print("<br />")
print("<h2>Password: " + password + "</h1>")
print("</body>")
print("</html>")