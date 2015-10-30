<?php
function redmineform_install_variables()
{
    variable_set('redmine_api_url','http://url.com/api');
    variable_set('redmine_api_user','Username');
    variable_set('redmine_api_password','');
    variable_set('redmine_api_project','redmine');
    variable_set('redmine_api_tip','Text der über dem Formular auftacht, z.B. mit Hinweis auf ein Forum oder den Bugtracker');
}
function redmineform_uninstall_variables()
{
    variable_del('redmine_api_url');
    variable_del('redmine_api_user');
    variable_del('redmine_api_password');
    variable_del('redmine_api_project');
    variable_del('redmine_api_tip');
}