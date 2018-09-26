from rest_framework import serializers
from configuration.models import *

class BotConfigSerializers(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = BotConfig
        depth = 1
        fields = ('id','url','bot_status', 'time_interval', 'max_lost', 'db_verbosity', 'currencies')

class CurrencySerializers(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Currency
        fields = ('id', 'url', 'name', 'symbol', 'description')




 