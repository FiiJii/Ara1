from django.db import models

# Create your models here.
class BotConfig(models.Model):
    bot_status = models.BooleanField(default=False)
    time_interval = models.IntegerField(default=0)
    max_lost = models.DecimalField(max_digits=19, decimal_places=10)


