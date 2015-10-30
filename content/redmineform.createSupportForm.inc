<?php


function redminesupport_form()
{
    $url = variable_get('redmine_api_url');
    $options = array();     
    $options['headers'] = array('Content-Type' => 'text/xml');
    $options['method'] = 'GET';
        
    $response = drupal_http_request($url.'projects.xml',$options);
    $xml = new SimpleXMLElement($response->data);
    $projektliste_identifier =  redminesupport_get_content($xml,"//*[local-name()='project']/*[local-name()='identifier']");
    $projektliste_name =  redminesupport_get_content($xml,"//*[local-name()='project']/*[local-name()='name']");
    $projektliste_ = array_combine($projektliste_identifier,$projektliste_name);
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
    
    if($tracker == '0')
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
    $form['redmine_ticket'] = array(
        '#type' => 'textarea',
        '#title' => t('Bitte geben Sie hier Ihre Meldung ein'),
        '#size' => 100,
        '#maxlength' => 10000,
        '#required' => TRUE,
    );
    $form['submit'] = array('#type' => 'submit', '#value' => t('Ticket erstellen'));
    return $form;
}


function redminesupport_form_submit($form, &$form_state) 
{
    drupal_set_message(t('The form has been submitted.'));
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