from django.shortcuts import HttpResponse,render
from django.http import JsonResponse

 

def home(request) :
    # Process data in Python
    return render(request,'project4app/index.html')

def login(request) :
    if request.method=="POST":
        if request.POST['email'] == 'admin@gmail.com' and request.POST['password'] == 'admin' :
            return  HttpResponse(('<h1>Welcome {} ..!</h1>').format(request.POST['email'].upper()))
    return render(request,'project4app/login.html')

def track(request) :
    # Process data in Python
    return render(request,'project4app/search.html')

def root(request) :
    if request.method == "POST" :
        frm = request.POST['from']
        to = request.POST['to']
        print(to)

    return render(request,'project4app/track.html')



