from rest_framework import serializers
from transactions.models import Transaction, TransactionDetail

class TransactionSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Transaction
        fields ='__all__'

class TransactionDetailSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = TransactionDetail
        fields ='__all__'