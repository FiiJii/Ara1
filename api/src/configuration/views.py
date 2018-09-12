from configuration.models import BotConfig
from .services import *
from rest_framework import viewsets
from django.core.exceptions import PermissionDenied
from configuration.serializers import BotConfigSerializers


# Create your views here.
class BotConfigView(viewsets.ModelViewSet):
    queryset = BotConfig.objects.all()
    serializer_class = BotConfigSerializers

    def perform_create(self, serializer):
        config_service = ConfigServicesBase()
        if config_service.can_config_bot(): 
           serializer.save()
        else:
            raise PermissionDenied("Existe una configuracion del Bot")
    

