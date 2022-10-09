/**********************************************************************************
*\file         paperback_cppscript_mgr.h
*\brief        paperback_cppscript_mgr.h
*
*\author	   Lee Jun Pu, 100% Code Contribution
*
*\copyright    Copyright (c) 2022 DigiPen Institute of Technology. Reproduction
                     or disclosure of this file or its contents without the prior
                     written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

namespace paperback::script
{
    //-----------------------------------
    //           Script Types
    //-----------------------------------
    namespace type
    {
        // Script Guid
        using guid = xcore::guid::unit<64, struct cppscript_tag>;

        // Script Descriptor
        struct info
        {
            const type::guid	 m_Guid;
            const char* const    m_pName;
        };

        // Script Type
        struct button
        {
            const type::guid  m_Guid{ };
            const char* const m_pName;
        };
    }


    //-----------------------------------
	//         Script Interface
	//-----------------------------------
    struct script_interface
    {
        PPB_FORCEINLINE
        script_interface                         ( paperback::coordinator::instance& Instance ) noexcept : m_Coordinator{ Instance } { }
        virtual void           Run               ( void )                                       noexcept                             { };
        virtual                ~script_interface ( void )                                       noexcept = default;

        paperback::coordinator::instance&       m_Coordinator;
    };


    //-----------------------------------
    //        Button Interface
    //-----------------------------------
    struct button_interface : script_interface
    {
        PPB_FORCEINLINE
        button_interface               ( paperback::coordinator::instance& Instance ) noexcept : script_interface{ Instance } { }
        virtual void Run               ( void )                                       noexcept                                { };   // Default
        virtual void OnClick           ( void )                                       noexcept                                { };   // On Mouse Click
        virtual void OnHover           ( void )                                       noexcept                                { };   // On Mouse Hover
        virtual void OnUnHover         ( void )                                       noexcept                                { };   // On Mouse Un-Hover
        virtual void OnPressed         ( void )                                       noexcept                                { };   // On Mouse Pressed
        virtual void OnReleased        ( void )                                       noexcept                                { };   // On Mouse Released
    };

    //-----------------------------------
    //         Card Interface
    //-----------------------------------
    struct card_interface : script_interface
    {
        PPB_FORCEINLINE
        card_interface                 ( paperback::coordinator::instance& Instance ) noexcept : script_interface{ Instance } { }
        virtual void Run               ( void )                                       noexcept                                { };   // Default
        virtual void OnClick           (paperback::component::entity& Entity, paperback::u32 PrefabGID, int PosIndex )     noexcept      { };   // On Mouse Click
        virtual void OnHover           ( void )                                       noexcept                                { };   // On Mouse Hover
        virtual void OnUnHover         ( void )                                       noexcept                                { };   // On Mouse Un-Hover
        virtual void OnPressed         ( void )                                       noexcept                                { };   // On Mouse Pressed
        virtual void OnReleased        ( void )                                       noexcept                                { };   // On Mouse Released
    };
    
    //-----------------------------------
    //         Behaviour Interface
    //-----------------------------------
    struct behaviour_interface : script_interface
    {
        PPB_FORCEINLINE
        behaviour_interface            (paperback::coordinator::instance& Instance) noexcept : script_interface{ Instance } { }
        virtual void Run               (void)                                       noexcept { };   // Default
        virtual void Init              (void)                                       noexcept { };   // On Init
        virtual void Update            (void)                                       noexcept { };   // On Update
        virtual void Destroy           (void)                                       noexcept { };   // On Destroy
        virtual void OnCollisionEnter  (void)                                       noexcept { };   // On Collision Enter
        virtual void OnCollisionStay   (void)                                       noexcept { };   // On Collision Stay
        virtual void OnCollisionExit   (void)                                       noexcept { };   // On Collision Exit
    };

    //-----------------------------------
	//        Create Script Info
	//-----------------------------------
    namespace details
    {
        template< paperback::concepts::Script T_SCRIPT >
        consteval script::type::info CreateInfo( void ) noexcept
        {
            return script::type::info
            {
                .m_Guid  = T_SCRIPT::typedef_v.m_Guid.m_Value
                           ? T_SCRIPT::typedef_v.m_Guid
                           : type::guid{ __FUNCSIG__ }
            ,   .m_pName = T_SCRIPT::typedef_v.m_pName
            };
        }

        template< typename T_CLASS >
        static constexpr auto info_v = details::CreateInfo<T_CLASS>();
    }

    template< typename T_CLASS >
    constexpr auto& info_v = details::info_v< T_CLASS >;
}


//-----------------------------------
//       CPP Script Manager
//-----------------------------------
namespace paperback::script
{
    struct manager
    {
        using CPPScriptsList   = std::vector< std::pair< const script::type::info*
                                                       , std::unique_ptr<paperback::script::script_interface> >>;
        using CPPScriptsMap    = std::unordered_map< paperback::u64
                                                   , paperback::script::script_interface* >;


        //-----------------------------------
        //             Default
        //-----------------------------------

        PPB_FORCEINLINE
        manager( paperback::coordinator::instance& Coordinator ) noexcept;


        //-----------------------------------
        //          Registration
        //-----------------------------------

        template < paperback::concepts::Script T_SCRIPT >
        void RegisterScript( void ) noexcept;

        template < paperback::concepts::Script... T_SCRIPT >
        void RegisterScripts( void ) noexcept;


        //-----------------------------------
        //             Search
        //-----------------------------------

        template < paperback::concepts::Script T_SCRIPT >
        T_SCRIPT* FindScript( const paperback::u64 ScriptGuid ) const noexcept;

        template < paperback::concepts::Script T_SCRIPT >
        T_SCRIPT& GetScript( const paperback::u64 ScriptGuid ) const noexcept;


        //-----------------------------------
        //             Getter
        //-----------------------------------

        PPB_FORCEINLINE
        const CPPScriptsList& GetScriptsList( void ) const noexcept;
        

        CPPScriptsList                       m_ScriptsList;
        CPPScriptsMap                        m_ScriptsMap;
        paperback::coordinator::instance&    m_Coordinator;
    };
}