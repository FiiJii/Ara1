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
    
    
    def add_coin(self, request, pk):
        current_config = BotConfig.objects.get(pk=pk)

        coin = Currency.objects.get(id=request.data['id'])

        current_config.currencies.add(coin)

        current_config.save()

        serializer = BotConfigSerializers(current_config, context={'request': request})
        
        return Response(status=status.HTTP_201_CREATED, data=serializer.data)
    
    def delete_coin(self, request, pk=None):
        current_config = BotConfig.objects.get(pk=pk)

        coin = Currency.objects.get(id=request.data['id'])

        current_config.currencies.remove(coin)

        serializer = BotConfigSerializers(current_config, context={'request': request})
        
        return Response(status=status.HTTP_204_NO_CONTENT, data=serializer.data)
         
    @action(methods=['post','delete'], detail=True)
    def coins(self, request, pk=None):
        methods={"POST":lambda: self.add_coin(request,pk),
                 "DELETE":lambda: self.delete_coin(request,pk)}
        return methods[request.method]()


class CurrencyView(viewsets.ModelViewSet):
    queryset = Currency.objects.all()
    serializer_class = CurrencySerializers

