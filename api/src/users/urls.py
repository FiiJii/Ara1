from django.conf.urls import url, include
from rest_framework import routers
from users import views

router = routers.DefaultRouter()
router.register(r'users', views.UserView)

urlpatterns = [
    url(r'^account/', include(router.urls))
]