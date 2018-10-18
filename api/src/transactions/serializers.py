from rest_framework import serializers
from transactions.models import Transaction, TransactionDetail

class TransactionNestedDetailSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = TransactionDetail
        fields = ('parity', 'amount', 'commission','action')

class TransactionSerializer(serializers.HyperlinkedModelSerializer):
    details = TransactionNestedDetailSerializer(many=True, read_only=True)
    class Meta:
        model = Transaction
        fields =('id','url','creation_date','investment', 'earnings', 'details')

class TransactionDetailSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = TransactionDetail
        fields =('id','url', 'transaction', 'parity', 'action','amount', 'commission','okex_order','transaction_order')
