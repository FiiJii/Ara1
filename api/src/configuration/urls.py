from django.conf.urls import url, include
from rest_framework import routers
from configuration import views

router = routers.DefaultRouter()
router.register(r'bot', views.BotConfigView)
router.register(r'currency', views.CurrencyView)
router.register(r'exchange', views.ExchangeView)
router.register(r'coin', views.CoinView)


urlpatterns = [
    url(r'^config/', include(router.urls))
]
