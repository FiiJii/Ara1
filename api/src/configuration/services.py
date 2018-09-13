from configuration.models import BotConfig


def get_config_bot_queryset():
    return BotConfig.objects.all()
    
def can_config_bot():
    return not get_config_bot_queryset().filter(bot_status=True).exists()