from configuration.models import *
from transactions.models import *
from .services import *
from rest_framework import viewsets, status, filters
from rest_framework.decorators import action
from rest_framework.response import Response
from rest_framework.exceptions import ValidationError
from django_filters.rest_framework import DjangoFilterBackend
from configuration.serializers import *
from api_trading.pagination import OptionalPagination
from configuration.filters import CoinsFilterBackend
from django.db.models import Sum, Avg
import datetime
from datetime import date, timedelta
from dateutil.relativedelta import relativedelta
import requests


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
    
    
    def add_pair(self, request, pk=None):
        current_config = BotConfig.objects.get(pk=pk)
        coin = Currency.objects.get(id=request.data['id'])
        current_config.currencies.add(coin)
        current_config.save()
        serializer = BotConfigSerializer(current_config, context={'request': request})        
        return Response(status=status.HTTP_201_CREATED, data=serializer.data)
    
    def delete_pair(self, request, pk=None):
        current_config = BotConfig.objects.get(pk=pk)
        coin = Currency.objects.get(id=request.data['id'])
        current_config.currencies.remove(coin)
        serializer = BotConfigSerializer(current_config, context={'request': request})        
        return Response(status=status.HTTP_204_NO_CONTENT, data=serializer.data)
    
    def list_pair(self, request, pk=None):
        current_config = BotConfig.objects.get(pk=pk)
        coins = current_config.currencies.all()
        serializer = CurrencySerializer(coins, many= True, context={'request': request})        
        return Response(status=status.HTTP_200_OK, data=serializer.data)

    def add_coins(self, request, pk=None):
        current_config = BotConfig.objects.get(pk=pk)
        qs=Currency.objects.filter(status='active')
        if request.data['symbol'] not in ['eth','usd','btc']:
            return Response(status=status.HTTP_400_BAD_REQUEST, data=serializer.data)
        qs=qs.filter(symbol__endswith=request.data['symbol'])
        for currency in qs:
            current_config.currencies.add(currency)
        current_config.save()
        serializer = BotConfigSerializer(current_config, context={'request': request})        
        return Response(status=status.HTTP_201_CREATED, data=serializer.data)
    
    def delete_coins(self, request, pk=None):
        current_config = BotConfig.objects.get(pk=pk)
        if request.data['symbol'] not in ['eth','usd','btc']:
            return Response(status=status.HTTP_400_BAD_REQUEST, data=serializer.data)
        qs=Currency.objects.filter(status='active')
        qs=qs.filter(symbol__endswith=request.data['symbol'])
        for currency in qs:
            current_config.currencies.remove(currency)
        serializer = BotConfigSerializer(current_config, context={'request': request})        
        return Response(status=status.HTTP_204_NO_CONTENT, data=serializer.data)
    
    def list_coins(self, request, pk=None):
        current_config = BotConfig.objects.get(pk=pk)
        coins = get_active_coins(current_config)
        serializer = CoinSerializer(coins, many= True, context={'request': request})        
        return Response(status=status.HTTP_200_OK, data=serializer.data)

    @action(methods=['get', 'post', 'delete'], detail=True)
    def pairs(self, request, pk=None):
        methods={"POST":lambda: self.add_pair(request,pk),
                 "DELETE":lambda: self.delete_pair(request,pk),
                 "GET":lambda: self.list_pair(request,pk)}
        return methods[request.method]()



    @action(methods=['get', 'post', 'delete'], detail=True)
    def coins(self, request, pk=None):
        methods={"POST":lambda: self.add_coins(request,pk),
                 "DELETE":lambda: self.delete_coins(request,pk),
                 "GET":lambda: self.list_coins(request,pk)}
        return methods[request.method]()

class CoinView(viewsets.ModelViewSet):
    queryset = Coin.objects.all()
    serializer_class = CoinSerializer
    pagination_class = OptionalPagination


class CurrencyView(viewsets.ModelViewSet):
    queryset = Currency.objects.all().order_by('id')
    filter_backends = (DjangoFilterBackend,filters.OrderingFilter,CoinsFilterBackend)
    serializer_class = CurrencySerializer
    filter_fields = ('symbol', 'status')
    pagination_class = OptionalPagination

    def perform_create(self, serializer):
        qs = self.queryset.filter(symbol=self.request.data['symbol'])
        if qs.exists():
            raise ValidationError('Currency already exists')
        serializer.save()

    @action(methods=['get'], detail=False)
    def averages(self, request):
        data=[]
        price_btc = get_price_btc();
        coins = self.paginate_queryset(Currency.objects.filter(status='active'))
        for coin in coins:            
            average_Tx_last_60second = TransactionDetail.objects.filter(transaction__creation_date__gte = datetime.datetime.now()-timedelta(seconds=60)).aggregate(total=Avg("amount"))["total"] or 0;
            average_Tx_last_hour = TransactionDetail.objects.filter(transaction__creation_date__gte = datetime.datetime.now()-timedelta(hours=1)).aggregate(total=Avg("amount"))["total"] or 0;         
            average_tx_last_6hours = TransactionDetail.objects.filter(transaction__creation_date__gte = datetime.datetime.now()-timedelta(hours=6)).aggregate(total=Avg("amount"))["total"] or 0;
            average_tx_last_12hours = TransactionDetail.objects.filter(transaction__creation_date__gte = datetime.datetime.now()-timedelta(hours=12)).aggregate(total=Avg("amount"))["total"] or 0;
            average_tx_last_24hours = TransactionDetail.objects.filter(transaction__creation_date__gte = datetime.datetime.now()-timedelta(hours=24)).aggregate(total=Avg("amount"))["total"] or 0;            
            ticker = get_ticker_coin(coin.symbol)

            if BotConfig.objects.filter(currencies__symbol = coin.symbol).exists():
                status = "active";
            else:
                status = "inactive";

            usd_volume = (float(price_btc) * float(ticker["vol"]));                   
            coin_data = {
                "id" : coin.id,
                "symbol" : coin.symbol,
                "name" : coin.name,
                "name_parity" : coin.name_symbol,
                "status_parity" : status,
                "last": ticker["last"],
                "bid" : ticker["buy"],
                "ask" : ticker["sell"], 
                "volume" : ticker["vol"],
                "usd_volume" : usd_volume,
                "average_tx_last_60second": average_Tx_last_60second,
                "average_tx_last_hour": average_Tx_last_hour,
                "average_tx_last_6hours": average_tx_last_6hours,
                "average_tx_last_12hours": average_tx_last_12hours,
                "average_tx_last_24hours": average_tx_last_24hours,
            }
            data.append(coin_data);           
        return self.get_paginated_response(data)

    
class ExchangeView(viewsets.ModelViewSet):
    queryset = Exchange.objects.all()
    serializer_class = ExchangeSerializer
    pagination_class = OptionalPagination

    def perform_create(self, serializer):
        qs = self.queryset.filter(name=self.request.data['name'])
        if qs.exists():
            raise ValidationError('Exchange already exists')
        serializer.save()
