# Inviwo Python script 
import urllib2, base64
USER = ""
PWD = ""
INVIWO_TEST_ROOT = ""

def setInviwoTestRootURL():
   global INVIWO_TEST_ROOT
   INVIWO_TEST_ROOT = "https://scivis.itn.liu.se/inviwo-regress/run"
 
def setInviwoAuthentication(username, password):
   global USER, PWD
   USER = username
   PWD = password
   setInviwoTestRootURL()
 
def downloadInviwoReferenceImage(ws, refOutputFile, canvasIndex):
   global USER
   global PWD
   global INVIWO_TEST_ROOT 
   
   d=ws.split('.')[0]
   inviwoURL = INVIWO_TEST_ROOT+'/'+d+'/ref-'+str(canvasIndex)+'.png'
   request = urllib2.Request(inviwoURL)
   base64string = base64.encodestring('%s:%s' % (USER, PWD)).replace('\n', '')
   request.add_header("Authorization", "Basic %s" % base64string)   
   result = urllib2.urlopen(request)

   if result.code == 200:
        output = open(refOutputFile,'wb')
        output.write(result.read())
        print 'Downloaded reference image from ' + inviwoURL
        output.close()
