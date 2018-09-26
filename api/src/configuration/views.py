from configuration.models import *
from .services import *
from rest_framework import viewsets, status
from rest_framework.decorators import action
from rest_framework.response import Response
from rest_framework.exceptions import ValidationError
from configuration.serializers import *


class BotConfigView(viewsets.ModelViewSet):
    queryset = BotConfig.objects.all()
    serializer_class = BotConfigSerializers
    
    def perform_create(self, serializer):
        if not can_config_bot():
            raise ValidationError('There is a configuration of the Bot')
        serializer.save()
    
    @action(methods=['post'], detail=True)  
    def add_coin(self, request, pk):
        """ obtener la configuracion actual"""
        current_config = BotConfig.objects.get(pk=pk)

        """ obtener la moneda """
        coin = Currency.objects.get(id=request.data['id'])

        """ agregar la moneda a la configuracion """
        current_config.currencies.add(coin)

        """ guardar la configuracion """
        current_config.save()

        serializer = BotConfigSerializers(current_config, context={'request': request})
        
        return Response(status=status.HTTP_201_CREATED, data=serializer.data)

class CurrencyView(viewsets.ModelViewSet):
    queryset = Currency.objects.all()
    serializer_class = CurrencySerializers

