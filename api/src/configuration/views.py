from configuration.models import BotConfig

# Create your views here.
class BotCongifView(viewsets.ModelViewSet):
    queryset = BotConfig.objects.all()