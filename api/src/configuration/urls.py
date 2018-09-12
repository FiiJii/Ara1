from django.conf.urls import url, include
from rest_framework import routers
from configuration import views

router = routers.DefaultRouter()
router.register(r'config', views.BotConfigView)


urlpatterns = [
    url(r'^', include(router.urls))
]