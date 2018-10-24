from rest_framework import serializers
from configuration.models import *

class BotConfigSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = BotConfig
        depth = 1
        fields = ('id','url','bot_status', 'time_interval', 'max_lost', 'db_verbosity', 'currencies')

class CurrencySerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Currency
        fields = ('id', 'url', 'name', 'symbol', 'name_symbol', 'description')


class ExchangeSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Exchange
        fields = ('id', 'url', 'name', 'status')

 