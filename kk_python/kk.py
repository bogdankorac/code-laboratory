#!/usr/bin/env python

# this is teaching example of webserver using sockets and jinja2 templates
# Sockets handling is reused from the code written by Stephen C Phillips (http://scphillips.com).

import socket
import re
import os
import shelve
import pickle
import time
import datetime
import string
from jinja2 import Environment, FileSystemLoader


#global variables
gConnections = 0
gServerUrl = "http://localhost:8887/"
DEFAULT_PAGE = "kk_form.html"

#setting environment for jinja2
PATH = os.path.dirname(os.path.abspath(__file__))
gTemplateEnvironment = Environment(
    autoescape=False,
    loader=FileSystemLoader(os.path.join(PATH, 'templates')),
    trim_blocks=False)

#functions
def render_template(template_filename, context):
    return gTemplateEnvironment.get_template(template_filename).render(context)

def fResponse2(page,params):
	context = {'params' : params,'home' : gServerUrl}
        print "context passed to fResponse2:"
        print context
	return render_template(page,context)

# Standard socket stuff:
host = '127.0.0.1' # do we need socket.gethostname() ?
port = 8887
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((host, port))
sock.listen(1) # don't queue up any requests

#some print
print("set path to %s"%PATH)

# Loop forever, listening for requests:
while True:
    csock, caddr = sock.accept()
    print "Connection from: " + `caddr`
    req = csock.recv(4096) # get the request, 4kB max
    print req

    #handling "GET"/default request
    if (req.split("/")[0] == "GET "):
        params = {}
        page = DEFAULT_PAGE

        #if there are parameters passed, we will parse them here

	# Return 200 OK an the page
	csock.sendall("HTTP/1.0 200 OK\n" + fResponse2(page,params))

    else:
        # If there was no recognised command then return a 404 (page not found)
        print "Returning 404"
        csock.sendall("HTTP/1.0 404 Not Found\n"+fResponse2("page_not_found.html",params))
    csock.close()
