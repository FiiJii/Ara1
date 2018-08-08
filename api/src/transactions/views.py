from transactions.models import Transaction, TransactionDetail
from rest_framework import viewsets
from transactions.serializers import TransactionSerializer, TransactionDetailSerializer

class TransactionViewSet(viewsets.ModelViewSet):
    queryset = Transaction.objects.all()
    serializer_class = TransactionSerializer
    

class TransactionDetailViewSet(viewsets.ModelViewSet):
    queryset = TransactionDetail.objects.all()
    serializer_class = TransactionDetailSerializer

