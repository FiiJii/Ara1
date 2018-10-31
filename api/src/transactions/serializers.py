from rest_framework import serializers
from transactions.models import Transaction, TransactionDetail
from configuration.serializers import CurrencySerializer
from api_trading.serializers import *


class TransactionNestedDetailSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = TransactionDetail
        fields = ('parity', 'amount', 'commission','action')

class TransactionSerializer(serializers.HyperlinkedModelSerializer):
    details = TransactionNestedDetailSerializer(many=True, read_only=True)
    class Meta:
        model = Transaction
        fields =('id','url','creation_date','investment', 'earnings', 'details')

class TransactionDetailSerializer(ExpandableModelSerializer):
    class Meta:
        model = TransactionDetail
        depth=1
        nested_classes={"parity":CurrencySerializer}
        fields =('id', 'url', 'transaction', 'creation_date', 'parity', 'transaction_order', 'okex_order', 'action', 'amount', 'commission')
