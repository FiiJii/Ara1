from configuration.models import BotConfig


class ConfigServicesBase:

    def get_config_bot_queryset(self):
        return BotConfig.objects.all()
    
    def can_config_bot(self):
        return self.get_config_bot_queryset().filter(bot_status=True).count() == 0