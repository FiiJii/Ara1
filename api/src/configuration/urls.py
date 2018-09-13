from django.conf.urls import url, include
from rest_framework import routers
from configuration import views

router = routers.DefaultRouter()
router.register(r'bot', views.BotConfigView)


urlpatterns = [
    url(r'^config/', include(router.urls))
]