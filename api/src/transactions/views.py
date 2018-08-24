from transactions.models import Transaction, TransactionDetail
from rest_framework import viewsets
from transactions.serializers import TransactionSerializer, TransactionDetailSerializer
from django_filters.rest_framework import DjangoFilterBackend
from transactions.filters import TransactionDateRangeFilter


class Transaction(viewsets.ModelViewSet):
    queryset = Transaction.objects.order_by('-creation_date')
    serializer_class = TransactionSerializer
    filterset_class = TransactionDateRangeFilter
  
    

class TransactionDetail(viewsets.ModelViewSet):
    queryset = TransactionDetail.objects.all()
    serializer_class = TransactionDetailSerializer

