from transactions.models import Transaction, TransactionDetail
from rest_framework import viewsets
from rest_framework import filters
from transactions.serializers import TransactionSerializer, TransactionDetailSerializer
from django_filters.rest_framework import DjangoFilterBackend
from transactions.filters import TransactionDateRangeFilter, CurrencyFilter
from rest_framework.decorators import list_route
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from rest_framework import status
from api_trading.pagination import OptionalPagination
from django.db.models import Sum, Avg
import datetime
from datetime import date, timedelta
from dateutil.relativedelta import relativedelta
from configuration.models import *
import requests

class TransactionView(viewsets.ModelViewSet):
    queryset = Transaction.objects.all().order_by('-creation_date')
    serializer_class = TransactionSerializer
    filterset_class = TransactionDateRangeFilter   
    pagination_class = OptionalPagination

    @list_route(methods=['get'])
    def max_earnings(self,request):
        queryset = self.filter_queryset(Transaction.objects.all().filter(earnings__gt=0)).order_by('-earnings')
        serializer = self.serializer_class(queryset, many=True, context={'request': request})
        return Response(status=status.HTTP_200_OK, data=serializer.data)
    
    @list_route(methods=['get'])
    def max_lost(self, request):
        queryset = self.filter_queryset(Transaction.objects.all().filter(earnings__lt=0)).order_by('earnings')
        serializer = self.serializer_class(queryset, many=True, context={'request': request})
        return Response(status=status.HTTP_200_OK, data=serializer.data)

class TransactionDetailView(viewsets.ModelViewSet):
    queryset = TransactionDetail.objects.all()
    filter_backends = (DjangoFilterBackend,filters.SearchFilter,filters.OrderingFilter)
    filter_fields = ('transaction','parity',)
    search_fields = ('parity',)
    ordering_fields=('id',)
    serializer_class = TransactionDetailSerializer
    filterset_class = CurrencyFilter
    pagination_class = OptionalPagination 

    @list_route(methods=['get'])
    def averages(self, request):
        data=[]
        for coin in Currency.objects.all():
            average_Tx_last_60second = self.filter_queryset(TransactionDetail.objects.filter(transaction__creation_date__gte = datetime.datetime.now()-timedelta(seconds=60))).aggregate(total=Avg("amount"))["total"] or 0;

            average_Tx_last_hour = self.filter_queryset(TransactionDetail.objects.filter(transaction__creation_date__gte = datetime.datetime.now()-timedelta(hours=1))).aggregate(total=Avg("amount"))["total"] or 0;
            
            average_tx_last_6hours = self.filter_queryset(TransactionDetail.objects.filter(transaction__creation_date__gte = datetime.datetime.now()-timedelta(hours=6))).aggregate(total=Avg("amount"))["total"] or 0;

            average_tx_last_12hours = self.filter_queryset(TransactionDetail.objects.filter(transaction__creation_date__gte = datetime.datetime.now()-timedelta(hours=12))).aggregate(total=Avg("amount"))["total"] or 0;

            average_tx_last_24hours = self.filter_queryset(TransactionDetail.objects.filter(transaction__creation_date__gte = datetime.datetime.now()-timedelta(hours=24))).aggregate(total=Avg("amount"))["total"] or 0;
            response=requests.get("https://www.okex.com/api/v1/ticker.do?symbol="+coin.symbol);
            ticker=response.json()["ticker"];
            coin_data = {
                "symbol":coin.symbol,
                "name":coin.name,
                "last":ticker["last"],
                "bid" :ticker["buy"],
                "ask" :ticker["sell"], 
                "volume" :ticker["vol"],
                "average_tx_last_60second": average_Tx_last_60second,
                "average_tx_last_hour": average_Tx_last_hour,
                "average_tx_last_6hours": average_tx_last_6hours,
                "average_tx_last_12hours": average_tx_last_12hours,
                "average_tx_last_24hours": average_tx_last_24hours,
            }
            data.append(coin_data);
        
        
        return Response(data=data);   
