from django.conf.urls import url, include
from rest_framework import routers
from transactions import views

router = routers.DefaultRouter()
router.register(r'transactions', views.TransactionView)
router.register(r'transaction_details', views.TransactionDetailView)

urlpatterns = [
    url(r'^trading/', include(router.urls))    
]