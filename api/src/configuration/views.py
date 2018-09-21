from configuration.models import *
from .services import *
from rest_framework import viewsets
from rest_framework.exceptions import ValidationError
from configuration.serializers import *


class BotConfigView(viewsets.ModelViewSet):
    queryset = BotConfig.objects.all()
    serializer_class = BotConfigSerializers
    
    def perform_create(self, serializer):
        if not can_config_bot():
            raise ValidationError('There is a configuration of the Bot')
        serializer.save()


class CurrencyView(viewsets.ModelViewSet):
    queryset = Currency.objects.all()
    serializer_class = CurrencySerializers

