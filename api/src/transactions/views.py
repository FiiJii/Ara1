from transactions.models import Transaction, TransactionDetail
from rest_framework import viewsets, status, filters
from transactions.serializers import TransactionSerializer, TransactionDetailSerializer
from django_filters.rest_framework import DjangoFilterBackend
from transactions.filters import TransactionDateRangeFilter, CurrencyFilter
from rest_framework.decorators import action
from rest_framework.permissions import IsAuthenticated
from rest_framework.response import Response
from api_trading.pagination import OptionalPagination

class TransactionView(viewsets.ModelViewSet):
    queryset = Transaction.objects.all().order_by('-creation_date')
    serializer_class = TransactionSerializer
    filterset_class = TransactionDateRangeFilter   
    pagination_class = OptionalPagination

    @action(methods=['get'], detail=False)
    def max_earnings(self,request):
        queryset = self.filter_queryset(Transaction.objects.all().filter(earnings__gt=0)).order_by('-earnings')
        serializer = self.serializer_class(queryset, many=True, context={'request': request})
        return Response(status=status.HTTP_200_OK, data=serializer.data)
    
    @action(methods=['get'], detail=False)
    def max_lost(self, request):
        queryset = self.filter_queryset(Transaction.objects.all().filter(earnings__lt=0)).order_by('earnings')
        serializer = self.serializer_class(queryset, many=True, context={'request': request})
        return Response(status=status.HTTP_200_OK, data=serializer.data)

class TransactionDetailView(viewsets.ModelViewSet):
    queryset = TransactionDetail.objects.all()
    filter_backends = (DjangoFilterBackend,filters.SearchFilter,filters.OrderingFilter)
    filter_fields = ('transaction',)
    search_fields = ('parity',)
    ordering_fields=('id',)
    serializer_class = TransactionDetailSerializer
    filterset_class = CurrencyFilter
    pagination_class = OptionalPagination 

