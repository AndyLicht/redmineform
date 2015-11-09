<?php
function redmineform_install_variables()
{
    variable_set('redmine_api_url','http://url.com/api');
    variable_set('redmine_api_user','Username');
    variable_set('redmine_api_password','');
    variable_set('redmine_api_project','redmine');
    variable_set('redmine_api_check','0');
    variable_set('redmine_api_tip','Text der über dem Formular auftaucht, z.B. mit Hinweis auf ein Forum oder den Bugtracker');
    variable_set('redmine_api_customfields','0');
    variable_set('redmine_api_customfields_tip_check','0');
    variable_set('redmine_api_sending_aslink',0);
    variable_set('redmine_api_sending_text','');
    variable_set('redmine_api_sending_check','0');
    variable_set('redmine_api_customfields_tip_text','Im Redminesystem werden Name und Emailadresse nicht angezeigt. Die Daten dienen zur Rückfragen, aktuell werden Sie noch nicht automatisch über Akutalisierungen zu Ihrem Ticket informiert.');
}
function redmineform_uninstall_variables()
{
    variable_del('redmine_api_sending_text');
    variable_del('redmine_api_sending_check');
    variable_del('redmine_api_aslink');
    variable_del('redmine_api_url');
    variable_del('redmine_api_user');
    variable_del('redmine_api_password');
    variable_del('redmine_api_project');
    variable_del('redmine_api_tip');
    variable_det('redmine_api_check');
    variable_del('redmine_api_customfields');
    variable_del('redmine_api_customfields_tip_check');
    variable_del('redmine_api_customfields_tip_text');
}