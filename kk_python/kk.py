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
from CreditUser import CCreditUser
from jinja2 import Environment, FileSystemLoader

#classes
class CRequestData(object):
	def __init__(self):
		self.iznos = 0
		self.ucesce = 0
		self.ime1 = ""
		self.ime2 = ""
		self.kamata1 = 0.0
		self.kamata2 = 0.0
		self.limit1 = 0
		self.limit2 = 0
		self.usrreport = 0
		self.yearreport = 0
		self.newcalc = 0

#global variables
gConnections = 0
gServerUrl = "http://localhost:8887/"
DEFAULT_PAGE = "kk_form.html"
DISPLAY_RESULT = "kk_display_result.html"

#setting environment for jinja2
PATH = os.path.dirname(os.path.abspath(__file__))
gTemplateEnvironment = Environment(
    autoescape=False,
    loader=FileSystemLoader(os.path.join(PATH, 'templates')),
    trim_blocks=False)


#functions
def render_template(template_filename, context):
    return gTemplateEnvironment.get_template(template_filename).render(context)

def fResponse2(page,params,user):
	context = {'user' : user, 'params' : params,'home' : gServerUrl}
        print "context passed to fResponse2:"
        print context
	return render_template(page,context)

def parseClientRequest(request,requestData):

	l = request.find("iznos")
	if(l > 0):
		try:
			requestData.iznos = int(request[(l + len("iznos") + 1) : (l + request[l : ].find("&"))])
		except:
			requestData.iznos = 0

	l = request.find("ucesce")
	if(l > 0):
		try:
			requestData.ucesce = int(request[(l + len("ucesce") + 1) : (l + request[l : ].find("&"))])
		except:
			requestData.ucesce = 0

	l = request.find("Ime1")
	if(l > 0):
		try:
			requestData.ime1 = request[(l + len("Ime1") + 1) : (l + request[l : ].find("&"))]
		except:
			requestData.ime1 = ""

	l = request.find("Ime2")
	if(l > 0):
		try:
			requestData.ime2 = request[(l + len("Ime2") + 1) : (l + request[l : ].find("&"))]
		except:
			requestData.ime2 = ""

	l = request.find("Kamata1")
	if(l > 0):
		try:
			requestData.kamata1 = float(request[(l + len("Kamata1") + 1) : (l + request[l : ].find("&"))])
		except:
			requestData.kamata1 = 0.0

	l = request.find("Kamata2")
	if(l > 0):
		try:
			requestData.kamata2 = float(request[(l + len("Kamata2") + 1) : (l + request[l : ].find("&"))])
		except:
			requestData.kamata2 = 0.0

	l = request.find("Limit1")
	if(l > 0):
		try:
			requestData.limit1 = float(request[(l + len("Limit1") + 1) : (l + request[l : ].find("&"))])
		except:
			requestData.limit1 = 0.0

	l = request.find("Limit2")
	if(l > 0):
		try:
			requestData.limit2 = float(request[(l + len("Limit2") + 1) : (l + request[l : ].find(" "))])
		except:
			requestData.limit2 = 0.0

	l = request.find("usrreport")
	if(l > 0):
		try:
			requestData.usrreport = int(request[(l + len("usrreport") + 1) : (l + request[l : ].find("&"))])
		except:
			requestData.usrreport = 0

	l = request.find("yearreport")
	if(l > 0):
		try:
			requestData.yearreport = int(request[(l + len("yearreport") + 1) : (l + request[l : ].find("&"))])
		except:
			requestData.yearreport = 0

	l = request.find("newcalc")
	if(l > 0):
		requestData.newcalc = 1
	else:
		requestData.newcalc = 0


	return


# Standard socket stuff:
host = '127.0.0.1' # do we need socket.gethostname() ?
port = 8887
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind((host, port))
sock.listen(1) # don't queue up any requests

#init request data
reqData = CRequestData()

#init credit users
usr1 = CCreditUser()
usr2 = CCreditUser()
usr3 = CCreditUser()
users = [usr1, usr2, usr3]

#pass helper functions to jinja template as globals
gTemplateEnvironment.globals['usr0_getAmmountOnYear'] = users[0].getAmmountOnYear
gTemplateEnvironment.globals['usr0_getAnuityOnYear'] = users[0].getAnuityOnYear

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
	parseClientRequest(req, reqData)

	if(reqData.newcalc == 1):
		# reset request data in case of nw calculation
		reqData.iznos = 0
		reqData.ucesce = 0
		reqData.ime1 = ""
		reqData.ime2 = ""
		reqData.kamata1 = 0
		reqData.kamata2 = 0
		reqData.limit1 = 0
		reqData.limit2 = 0
		reqData.usrreport = 0
		reqData.yearreport = 0
		reqData.newcalc = 0
		page = DEFAULT_PAGE

	if(True):
		#debug information
		print ("iznos:%d")%reqData.iznos
		print ("ucesce:%d")%reqData.ucesce
		print ("Ime1:%s")%reqData.ime1
		print ("Ime2:%s")%reqData.ime2
		print ("Kamata1:%f")%reqData.kamata1
		print ("Kamata2:%f")%reqData.kamata2
		print ("Limit1:%f")%reqData.limit1
		print ("Limit2:%f")%reqData.limit2
		print ("usrreport:%d")%reqData.usrreport
		print ("yearreport:%d")%reqData.yearreport
		print ("newcalc:%d")%reqData.newcalc

	# setting request data which will be passed to jinja template		
	params["iznos"] = reqData.iznos
	params["ucesce"] = reqData.ucesce
	params["Ime1"] = reqData.ime1
	params["Ime2"] = reqData.ime2
	params["Kamata1"] = reqData.kamata1
	params["Kamata2"] = reqData.kamata2
	params["Limit1"] = reqData.limit1
	params["Limit2"] = reqData.limit2
	params["usrreport"] = reqData.usrreport
	params["yearreport"] = reqData.yearreport
	params["newcalc"] = reqData.newcalc	

	#doing calculation in case of one user
	if(reqData.ime1 != ""):
		users[0].AddName(reqData.ime1);
		if((reqData.kamata1 > 100) or (reqData.kamata1 < 0)):
			reqData.kamata1 = 1
		users[0].setRate(reqData.kamata1)
		users[0].setAnuityLimit(reqData.limit1)
		users[0].setTotalAmmount(reqData.iznos - reqData.ucesce)
		users[0].calculateTotalAnuityOnRate()
		users[0].calculateRateOnAmmount()
		page = DISPLAY_RESULT


	# Return 200 OK an the page
	csock.sendall("HTTP/1.0 200 OK\n" + fResponse2(page,params,users[0]))

    else:
        # If there was no recognised command then return a 404 (page not found)
        print "Returning 404"
        csock.sendall("HTTP/1.0 404 Not Found\n"+fResponse2("page_not_found.html",params,users[0]))
    csock.close()
