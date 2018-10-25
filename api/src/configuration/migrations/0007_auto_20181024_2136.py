# Generated by Django 2.1 on 2018-10-24 21:36

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('configuration', '0006_currency_name_symbol'),
    ]

    operations = [
        migrations.AddField(
            model_name='currency',
            name='status',
            field=models.CharField(choices=[('active', 'active'), ('inactive', 'inactive')], default='active', max_length=100),
            preserve_default=False,
        ),
        migrations.AlterField(
            model_name='botconfig',
            name='db_verbosity',
            field=models.CharField(choices=[('silent', 'silent'), ('medium', 'medium')], max_length=100),
        ),
    ]
