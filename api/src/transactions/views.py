from transactions.models import Transaction, TransactionDetail
from rest_framework import viewsets
from transactions.serializers import TransactionSerializer, TransactionDetailSerializer
from django_filters import rest_framework as filters

class TransactionFilter(filters.FilterSet):
    
    class Meta:
        model = Transaction
        fields = {
            'creation_date': ['iexact', 'lte', 'gte']
        }

class Transaction(viewsets.ModelViewSet):
    queryset = Transaction.objects.all().order_by('-creation_date')
    serializer_class = TransactionSerializer
    filterset_class = TransactionFilter
  
    

class TransactionDetail(viewsets.ModelViewSet):
    queryset = TransactionDetail.objects.all()
    serializer_class = TransactionDetailSerializer

