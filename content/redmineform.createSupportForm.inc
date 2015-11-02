<?php


function redminesupport_form()
{
    $url = variable_get('redmine_api_url');
    $options = array();     
    $options['headers'] = array('Content-Type' => 'text/xml');
    $options['method'] = 'GET';
        
    $response = drupal_http_request($url.'projects.xml',$options);
    $xml = new SimpleXMLElement($response->data);
    $projektliste_id =  redminesupport_get_content($xml,"//*[local-name()='project']/*[local-name()='id']");
    $projektliste_name =  redminesupport_get_content($xml,"//*[local-name()='project']/*[local-name()='name']");
    $projektliste_ = array_combine($projektliste_id,$projektliste_name);
    $projektliste_final = redminesupport_useFilter($projektliste_);
    
    
    $form = array();
    $form['redmine_introduction'] = array(
        '#type' => 'markup',
        '#markup' => variable_get('redmine_api_tip'),
    );
    $form['redmine_api_project'] = array(
        '#type' => 'select',
        '#title' => t('Projekt: '),
        '#options' => $projektliste_final,
        '#required' => False,
    );
    $tracker = variable_get('redmine_api_tracker');
    
    if($tracker == '1')
    {
        $response = drupal_http_request($url.'trackers.xml',$options);
        $xml = new SimpleXMLElement($response->data);
        $trackerliste_identifier =  redminesupport_get_content($xml,"//*[local-name()='tracker']/*[local-name()='id']");
        $trackerliste_name =  redminesupport_get_content($xml,"//*[local-name()='tracker']/*[local-name()='name']");
        $trackerliste = array_combine($trackerliste_identifier,$trackerliste_name);
        
       $form['redmine_api_tracker'] = array(
            '#type' => 'select',
            '#title' => t('Tracker: '),
            '#options' => $trackerliste,
            '#required' => TRUE,
        );
    }
    $form['redmine_ticket_subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Betreff:'),
        '#size' => 83,
        '#maxlength' => 255,
        '#required' => TRUE,
    );
    $form['redmine_ticket_body'] = array(
        '#type' => 'textarea',
        '#title' => t('Bitte geben Sie hier Ihre Meldung ein'),
        '#size' => 100,
        '#maxlength' => 10000,
        '#required' => TRUE,
    );
     $form['redmine_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name:'),
        '#size' => 83,
        '#maxlength' => 255,
        '#required' => FALSE,
    );
    $form['redmine_ticket_email'] = array(
        '#type' => 'textfield',
        '#title' => t('E-Mail:'),
        '#size' => 83,
        '#maxlength' => 255,
        '#required' => FALSE,
    );
    $form['submit'] = array('#type' => 'submit', '#value' => t('Ticket erstellen'));
    return $form;
}
/*
function redminesupport_form_validate($form, &$form_state)
{
    $mail = $form_values['submitted_tree']['email_address'];
    if (!valid_email_address($mail)) 
    {
       form_set_error('submitted][email_address', t('The email address appears to be invalid.'));
    }
}*/
function redminesupport_form_submit($form, &$form_state) 
{
    $url = variable_get('redmine_api_url').'issues.json';
    dsm(variable_get('redmine_api_user'));
    dsm(variable_get('redmine_api_password'));
    
    $json['issue']['project_id'] = $form_state['values']['redmine_api_project'];
    $json['issue']['subject'] = $form_state['values']['redmine_ticket_subject'];
    if(isset($form_state['values']['redmine_api_tracker']))
    {
        $json['issue']['tracker_id'] = $form_state['values']['redmine_api_tracker'];
    }
    $json['issue']['description'] = $form_state['values']['redmine_ticket_body'];
    $json['issue']['is_private'] = variable_get('redmine_api_publish');
    dsm(json_encode($json));
    
    $options['method'] = 'POST';
    $options['data'] = json_encode($json);
    $options['max_redirects'] = 0;      
    $options['headers']['Authorization'] = 'Basic ' . base64_encode(variable_get('redmine_api_user') .":". (variable_get('redmine_api_password')));
    $options['headers']['Content-Type'] = 'application/json';
    $options['headers']['Accept'] = 'application/json';
    $response = drupal_http_request($url, $options);
    
    dsm($response);
    
 
}



function redminesupport_get_content($xml,$xpath) //Achtung Funktion kann zwei Wertetypen ausgeben String oder Array[String]
{
    $content = array();
    $inhalte = $xml->xpath($xpath);
    if(is_array($inhalte) && count($inhalte)>0)
    {
        foreach ($inhalte as $inhalt)
        {
            if($inhalt!=='' && $inhalt != null)
            {
                array_push($content,(string)$inhalt);
            }
        }
        return $content;
    }
    else
    {
        return null;
    }
}

function redminesupport_useFilter($allProjects)
{   
    $inputProjects = variable_get('redmine_api_project');
    //wenn keine Projekte angegeben sind werden alle zur Auswahl angeboten
    if(null == $inputProjects)
    {
        return $allProjects;
    }
    else
    {
        
        $_array = array();
        if(strpos($inputProjects,','))
        {
            $_array = explode(',',$inputProjects);
        }
        else
        {
            $_array[0] = $inputProjects;
        };
        //bei 0 werden nur die angegebenen Projekte angezeigt; bei 1 werden diese rausgefiltert
        if(variable_get('redmine_api_which') == '0')
        {
            foreach ($allProjects as $project => $value) 
            {
                if(!in_array($project,$_array))
                {
                    unset($allProjects[$project]);
                }
            }
        }
        else
        {
            foreach ($_array as $project) 
            {
                if(array_key_exists($project,$allProjects))
                {
                    unset($allProjects[$project]);
                }
            }
        }
        return $allProjects;
    }  
}