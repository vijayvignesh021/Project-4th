from django.shortcuts import HttpResponse,render
from django.http import JsonResponse

 

def home(request) :
    # Process data in Python
    return render(request,'project4app/index.html')

def login(request) :
    # Process data in Python
    return render(request,'project4app/login.html')

def track(request) :
    # Process data in Python
    return render(request,'project4app/track.html')

