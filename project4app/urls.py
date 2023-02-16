from django.contrib import admin
from django.urls import path,include
from . import views
urlpatterns = [
    path('', views.home, name='home'),
    path('login/', views.login, name='login'),
    path('track/', views.track, name='track'),
    path('root/', views.root, name='root'),
]