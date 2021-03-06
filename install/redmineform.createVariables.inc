<?php
function redmineform_install_variables()
{
    variable_set('redmine_api_url','http://url.com/api');
    variable_set('redmine_api_debug',0);
    variable_set('redmine_api_user','Username');
    variable_set('redmine_api_password','');
    variable_set('redmine_api_project','redmine');
    variable_set('redmine_api_tracker','');
    variable_set('redmine_api_trackerstatus','');
    variable_set('redmine_api_tracker_switcher',0);
    variable_set('redmine_api_check','0');
    variable_set('redmine_api_tip','Text der über dem Formular auftaucht, z.B. mit Hinweis auf ein Forum oder den Bugtracker');
    variable_set('redmine_api_customfields','0');
    variable_set('redmine_api_customfields_tip_check','0');
    variable_set('redmine_api_sending_aslink',0);
    variable_set('redmine_api_sending_text','');
    variable_set('redmine_api_sending_check','0');
    variable_set('redmine_api_customfields_tip_text','Im Redminesystem werden Name und Emailadresse nicht angezeigt. Die Daten dienen zur Rückfragen, aktuell werden Sie noch nicht automatisch über Akutalisierungen zu Ihrem Ticket informiert.');
    variable_set('redmine_api_file_check','0');
    variable_set('redmine_api_file_type','jpeg,jpg,gif,bmp');
    variable_set('redmine_api_file_size',0);
}
function redmineform_uninstall_variables()
{   
    variable_del('redmine_api_tracker_switcher');
    variable_del('redmine_api_debug');
    variable_del('redmine_api_file_size');
    variable_del('redmine_api_file_type');
    variable_del('redmine_api_file_check');
    variable_del('redmine_api_sending_text');
    variable_del('redmine_api_sending_check');
    variable_del('redmine_api_aslink');
    variable_del('redmine_api_url');
    variable_del('redmine_api_user');
    variable_del('redmine_api_password');
    variable_del('redmine_api_project');
    variable_del('redmine_api_tracker');
    variable_del('redmine_api_trackerstatus');
    variable_del('redmine_api_tip');
    variable_det('redmine_api_check');
    variable_del('redmine_api_customfields');
    variable_del('redmine_api_customfields_tip_check');
    variable_del('redmine_api_customfields_tip_text');
}