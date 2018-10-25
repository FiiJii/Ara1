from configuration.models import *
from .services import *
from rest_framework import viewsets, status
from rest_framework.decorators import action
from rest_framework.response import Response
from rest_framework.exceptions import ValidationError
from django_filters.rest_framework import DjangoFilterBackend
from configuration.serializers import *
from api_trading.pagination import OptionalPagination


class BotConfigView(viewsets.ModelViewSet):
    queryset = BotConfig.objects.all()
    serializer_class = BotConfigSerializer
    filter_backends = (DjangoFilterBackend,)
    filter_fields = ('bot_status',)
    pagination_class = OptionalPagination
    
    def perform_create(self, serializer):
        if not can_config_bot():
            raise ValidationError('There is a configuration of the Bot')
        serializer.save()
    
    
    def add_coin(self, request, pk=None):
        current_config = BotConfig.objects.get(pk=pk)
        coin = Currency.objects.get(id=request.data['id'])
        current_config.currencies.add(coin)
        current_config.save()
        serializer = BotConfigSerializer(current_config, context={'request': request})        
        return Response(status=status.HTTP_201_CREATED, data=serializer.data)
    
    def delete_coin(self, request, pk=None):
        current_config = BotConfig.objects.get(pk=pk)
        coin = Currency.objects.get(id=request.data['id'])
        current_config.currencies.remove(coin)
        serializer = BotConfigSerializer(current_config, context={'request': request})        
        return Response(status=status.HTTP_204_NO_CONTENT, data=serializer.data)
    
    def list_coin(self, request, pk=None):
        current_config = BotConfig.objects.get(pk=pk)
        coins = current_config.currencies.all()
        serializer = CurrencySerializer(coins, many= True, context={'request': request})        
        return Response(status=status.HTTP_200_OK, data=serializer.data)

        
    @action(methods=['get', 'post', 'delete'], detail=True)
    def coins(self, request, pk=None):
        methods={"POST":lambda: self.add_coin(request,pk),
                 "DELETE":lambda: self.delete_coin(request,pk),
                 "GET":lambda: self.list_coin(request,pk)}
        return methods[request.method]()


class CurrencyView(viewsets.ModelViewSet):
    queryset = Currency.objects.all()
    serializer_class = CurrencySerializer
    pagination_class = OptionalPagination

    def perform_create(self, serializer):
        qs = self.queryset.filter(symbol=self.request.data['symbol'])
        if qs.exists():
            raise ValidationError('Currency already exists')
        serializer.save()
    
class ExchangeView(viewsets.ModelViewSet):
    queryset = Exchange.objects.all()
    serializer_class = ExchangeSerializer
    pagination_class = OptionalPagination

    def perform_create(self, serializer):
        qs = self.queryset.filter(name=self.request.data['name'])
        if qs.exists():
            raise ValidationError('Exchange already exists')
        serializer.save()
