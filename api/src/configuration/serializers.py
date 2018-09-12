from rest_framework import serializers
from configuration.models import BotConfig

class BotConfigSerializers(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = BotConfig
        fields = ('bot_status', 'time_interval', 'max_lost')

 