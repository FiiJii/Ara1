from django.conf.urls import url, include
from django.contrib import admin
from django.urls import path
from rest_framework_simplejwt.views import (
    TokenObtainPairView,
    TokenRefreshView,
)

urlpatterns = [
    path('admin/', admin.site.urls),
    url(r'^api/auth/token/$', TokenObtainPairView.as_view(), name='token_obtain_pair'),
    url(r'^api/auth/token/refresh/$', TokenRefreshView.as_view(), name='token_refresh'),
    url(r'^api/auth/', include('rest_framework.urls', namespace='rest_framework')),
    url(r'^api/', include('transactions.urls')),
    url(r'^api/', include('configuration.urls'))
]

