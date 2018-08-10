from rest_framework import serializers
from transactions.models import Transaction, TransactionDetail

class TransactionNestedDetailSerializer(serializers.ModelSerializer):
    class Meta:
        model = TransactionDetail
        fields = ('parity', 'amount', 'commission')

class TransactionSerializer(serializers.HyperlinkedModelSerializer):
    details = TransactionNestedDetailSerializer(many=True, read_only=True)
    class Meta:
        model = Transaction
        fields =('id','creation_date', 'details')

class TransactionDetailSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = TransactionDetail
        fields =('id', 'transaction', 'parity', 'amount', 'commission','okex_order')