from django.shortcuts import HttpResponse,render
import geocoder
from geopy.geocoders import Nominatim

geolocator = Nominatim(user_agent="geoapiExercises")
 

def home(request) :
    adr = geocoder.ip('me')
    latitude = adr.latlng[0]
    longitude =  adr.latlng[1]
    city = get_city(latitude, longitude)
    location  = city
    return render(request,'project4app/index2.html',{'loc':location})

def get_city(latitude, longitude):
    location = geolocator.reverse(f"{latitude}, {longitude}", exactly_one=True)
    return location.raw['address'].get('town', location.raw['address'].get('city', ''))
