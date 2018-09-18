from configuration.models import *
from .services import *
from rest_framework import viewsets
from rest_framework.exceptions import ValidationError
from configuration.serializers import BotConfigSerializers


class BotConfigView(viewsets.ModelViewSet):
    queryset = BotConfig.objects.all()
    serializer_class = BotConfigSerializers
    
    def perform_create(self, serializer):
        if not can_config_bot():
            raise ValidationError('Existe una configuracion del Bot')
        serializer.save()
