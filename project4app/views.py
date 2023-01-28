from django.shortcuts import HttpResponse,render
 

def home(request) :
 
    return render(request,'project4app/index2.html')
