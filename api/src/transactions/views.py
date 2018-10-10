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


class TransactionView(viewsets.ModelViewSet):
    queryset = Transaction.objects.all().order_by('-creation_date')
    filter_backends = (DjangoFilterBackend,filters.SearchFilter,)
    search_fields = ('details__parity',)
    serializer_class = TransactionSerializer
    filterset_class = TransactionDateRangeFilter
    

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
    filter_fields = ('transaction','parity',)
    search_fields = ('parity',)
    ordering_fields=('id',)
    filter_backends = (DjangoFilterBackend,filters.SearchFilter,filters.OrderingFilter)
    serializer_class = TransactionDetailSerializer
    filterset_class = CurrencyFilter
   
