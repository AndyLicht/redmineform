<?php

function redmineapi_configure()
{
    $form = array();
    $form['introduction'] = array(
        '#title' => t('Hilfe'),
        '#type' => 'fieldset',
        '#collapsible' => false,
        '#collapsed' => false,        
    );
    $form['introduction']['text'] = array(
        '#type' => 'markup',
        '#markup' => t('Bitte setzen Sie hier Ihre Parameter für die entsprechende Redmine Instanz. Sollte Ihr Server hinter einem Proxy stehen installieren Sie sich bitte zusätlich das http_proxy Modul.<p>'),
    );
   
    $form['advanced'] = array(
        '#type' => 'vertical_tabs',
        '#title' => t('Advanced settings'),
    );
    $form['redmine'] = array(
        '#type' => 'fieldset',
        '#title' => t('Settings'),
        '#group' => 'advanced',
    );
    $form['redmine']['redmine_api_url'] = array(
        '#type' => 'textfield',
        '#title' => t('URL zum Redmine API Endpunkt'),
        '#default_value' => variable_get('redmine_api_url'),
        '#size' => 100,
        '#maxlength' => 100,
        '#required' => FALSE,
    );
    $form['redmine']['redmine_api_user'] = array(
        '#type' => 'textfield',
        '#title' => t('Benutzername'),
        '#default_value' => variable_get('redmine_api_user'),
        '#size' => 100,
        '#maxlength' => 100,
        '#required' => FALSE,
    );
    $form['redmine']['redmine_api_password'] = array(
        '#type' => 'password',
        '#title' => t('Passwort'),
        '#default_value' => variable_get('redmine_api_password'),
        '#size' => 100,
        '#maxlength' => 100,
        '#required' => FALSE,
    );
    $form['redmine']['redmine_api_project'] = array(
        '#type' => 'textfield',
        '#title' => t('Projekt-IDs'),
        '#default_value' => variable_get('redmine_api_project'),
        '#size' => 100,
        '#maxlength' => 100,
        '#required' => FALSE,
        '#description' => t('Wenn das Feld leer ist werden alle Projekte der Redmineinstanz zur Verfügung gestellt, die Trennung erfolgt mit [,] zum Beispiel 1,5)'),
    );
    $form['redmine']['redmine_api_which'] = array(
        '#type' => 'select',
        '#title' => t('Angegebene Projekte'),
        '#default_value' => variable_get('redmine_api_which'),
        '#options' => array(0 => t('anzeigen'), 1 => t('nicht anzeigen')),
    );
    $form['redmine']['redmine_api_customfields'] = array(
        '#type' => 'checkbox',
        '#title' => t('Name und Emailadresse mit angeben'),
        '#default_value' => variable_get('redmine_api_customfields'),
    );
    $form['redmine']['redmine_api_tracker'] = array(
        '#type' => 'checkbox',
        '#title' => t('Möglichkeit den Tracker auszuwählen'),
        '#default_value' => variable_get('redmine_api_tracker'),
    );
    $form['redmine']['redmine_api_publish'] = array(
        '#type' => 'checkbox',
        '#title' => t('Tickets werden als private Tickets abgespeichert'),
        '#default_value' => variable_get('redmine_api_publish'),
    );
    $form['tipsettings'] = array(
        '#type' => 'fieldset',
        '#title' => t('Tip'),
        '#group' => 'advanced',
    );
    $form['tipsettings']['redmine_api_tip'] = array(
        '#type' => 'textarea',
        '#title' => t('Hinweistext (wird über dem Formularfeld angezeigt)'),
        '#default_value' => variable_get('redmine_api_tip'),
        '#size' => 100,
        '#maxlength' => 100,
        '#required' => FALSE,
    );
    return system_settings_form($form);
}