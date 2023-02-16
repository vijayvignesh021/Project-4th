

import os
import sys

path= '/home/vijayvignesh21/Project-4th'
if path not in sys.path:
    sys.path.append(path)


os.environ['DJANGO_SETTINGS_MODULE']  =  'project4.settings'

from django.core.wsgi import get_wsgi_application
application = get_wsgi_application()
